# Copyright 2019 Dietrich Epp.
# This file is licensed under the terms of the MIT license. See LICENSE.txt
# for details.
"""Keycode table generation functions."""
import collections
import csv
import os
import re

from common import Error


class ReadFile:
    """Context manager for reading input files.

    This will decorate exceptions inside the context with the filename.
    """

    def __init__(self, dirname, filename):
        try:
            fp = open(os.path.join(dirname, filename))
        except FileNotFoundError:
            raise Error("Data file not found", filename=filename)
        except OSError as ex:
            raise Error("Could not open data file: {}".format(ex),
                        filename=filename)
        self.filename = filename
        self.fp = fp

    def __enter__(self):
        return self.fp

    def __exit__(self, exc_type, exc_value, exc_tb):
        self.fp.close()
        if isinstance(exc_value, Error):
            if exc_value.filename is None:
                exc_value.filename = self.filename


Keycode = collections.namedtuple("Keycode", ["code", "name", "displayname"])

VALID_NAME = re.compile(r"[_a-z0-9]*(?: [_a-z0-9]+)*", re.IGNORECASE)
VALID_DISPLAYNAME = re.compile(r"[!-~]+(?: [!-~]+)*")


def read_hid(fp):
    """Read the HID keycode table.

    Arguments:
      fp: Input file
    Returns:
      Alist of Keycode objects
    """
    codes = set()
    names = set()
    displaynames = set()
    result = []
    reader = csv.reader(fp)

    def error(msg):
        return Error(msg, lineno=lineno)

    row = next(reader)
    headers = ["Keycode", "Name", "Display Name"]
    if row != headers:
        raise Error("Got headers {!r}, expected {!r}".format(row, headers),
                    lineno=1)
    for lineno, row in enumerate(reader, 2):
        if not row:
            continue
        try:
            codestr, name, displayname = row
        except ValueError:
            raise error("Got {} columns, expected 3".format(len(row)))
        try:
            code = int(codestr, 0)
        except ValueError:
            raise error("Invalid keycode {}".format(rowstr))
        if code in codes:
            raise error("Duplicate code {}".format(code))
        codes.add(code)
        if not name:
            if displayname:
                raise error("Name is empty but display name is present")
            continue
        if not VALID_NAME.fullmatch(name):
            raise error("Invalid name {!r}".format(name))
        if name in names:
            raise error("Duplicate name {!r}".format(name))
        names.add(name)
        if not displayname:
            displayname = name
        if not VALID_DISPLAYNAME.fullmatch(displayname):
            raise error("Invalid display name {!r}".format(displayname))
        if displayname in displaynames:
            raise error("Duplicate display name {!r}".format(displayname))
        result.append(Keycode(code, name, displayname))
    return result


Scancode = collections.namedtuple("Scancode", ["code", "name"])


def read_scancodes(fp):
    """Read a scancodes table mapping platform-specific scancodes to names.

    If a scancode appears twice in the file, only the first entry is included.
    Entries with empty names are filtered out.

    Arguments:
      fp: Input file
    Returns:
      A list of Scancode objects
    """
    result = []
    codes = set()
    names = set()
    reader = csv.reader(fp)

    def error(msg):
        return Error(msg, lineno=lineno)

    row = next(reader)
    headers = ["Keycode", "Name"]
    if row != headers:
        raise Error("Got headers {!r}, expected {!r}".format(row, headers),
                    lineno=1)
    for lineno, row in enumerate(reader, 2):
        if not row:
            continue
        if len(row) != 2:
            raise error("Got {} columns, expected 2".format(len(row)))
        codestr, name = row
        try:
            code = int(codestr)
        except ValueError:
            raise error("Invalid scancode value {!r}".format(codestr))
        if not VALID_NAME.fullmatch(name):
            raise error("Invalid scancode name {!r}".format(name))
        if name and name in names:
            raise error("Duplicate scancode name {!r}".format(name))
        names.add(name)
        if code not in codes:
            codes.add(code)
            if name:
                result.append(Scancode(code, name))
    return result


def read_names(fp):
    """Read a display name table mapping HID names to platform-specific names.

    Arguments:
      fp: Input file
    Returns:
      A dictionary mapping HID names to display names
    """
    result = {}
    reader = csv.reader(fp)

    def error(msg):
        return Error(msg, lineno=lineno)

    row = next(reader)
    headers = ["Name", "Display Name"]
    if row != headers:
        raise Error("Got headers {!r}, expected {!r}".format(row, headers),
                    lineno=1)
    for lineno, row in enumerate(reader, 2):
        if not row:
            continue
        try:
            name, displayname = row
        except ValueError:
            raise error("Got {} columns, expected 2".format(len(row)))
        if not name:
            continue
        if not VALID_NAME.fullmatch(name):
            raise error("Invalid name {!r}".format(name))
        if name in result:
            raise error("Duplicate name {!r}".format(name))
        if not displayname:
            continue
        if not VALID_DISPLAYNAME:
            raise error("Invalid display name {!r}".format(displayname))
        result[name] = displayname
    return result


class KeymapBuilder:
    """Platform-specific scancode to HID keycode map builder.

    Attributes:
      scancode_map: Map from platform scancode name to value
      used: Set of used scancode names
      keymap: Map from scancode to Keycode objects
      hid_names: Map from name to Keycode for all HID keycodes
    """

    def __init__(self, scancodes, hid_names):
        scancode_map = {}
        for code, name in scancodes:
            scancode_map[name] = code
        self.scancode_map = scancode_map
        self.used = set()
        self.keymap = {}
        self.hid_names = hid_names

    def apply_keymap(self, fp):
        """Apply the rules in a keymap file."""
        reader = csv.reader(fp)
        row = next(reader)
        headers = ["Platform Name", "HID Name"]
        if row != headers:
            raise Error("Got headers {!r}, expected {!r}".format(row, headers),
                        lineno=1)
        for lineno, row in enumerate(reader, 2):
            if not row:
                continue
            try:
                self.apply_row(row)
            except Error as ex:
                ex.lineno = lineno
                raise ex

    def apply_row(self, row):
        """Apply the rule in a single row of a keymap file."""
        try:
            match, name = row
        except ValueError:
            raise Error("Got {} columns, expected 2".format(len(row)))
        if match.startswith("/"):
            self.apply_regex(match, name)
        else:
            self.apply_single(match, name)

    def apply_regex(self, match, name):
        """Apply a regulare expression rule."""
        if len(match) <= 2 or not match.endswith("/"):
            raise Error("Invalid regular expression {!r}".format(match))
        try:
            regex = re.compile(match[1:-1])
        except ValueError as ex:
            raise Error("Invalid regular expression {!r}: {}".format(
                match, ex))
        used_size = len(self.used)
        for sname in set(self.scancode_map).difference(self.used):
            m = regex.fullmatch(sname)
            if m is None:
                continue
            if not name:
                self.used.add(sname)
                continue
            hname = m.expand(name)
            hid_key = self.hid_names.get(hname)
            if hid_key is None:
                continue
            code = self.scancode_map[sname]
            self.keymap[code] = hid_key
            self.used.add(sname)
        if len(self.used) == used_size:
            raise Error("No scancodes match {!r}".format(match))

    def apply_single(self, match, name):
        """Apply a direct mapping rule."""
        code = self.scancode_map.get(match)
        if code is None:
            raise Error("No scancode has name {!r}".format(match))
        if match in self.used:
            raise Error("Scancode {!r} already has mapping".format(match))
        self.used.add(match)
        if not name:
            return
        hid_key = self.hid_names.get(name)
        if hid_key is None:
            raise Error("No HID key is named {!r}".format(name))
        self.keymap[code] = hid_key


class Keytable:
    """A keycode table for a platform.

    Attributes:
      name: Platform name
      scancodes: List of Scancode objects
      displaynames: List of (keycode, name) mapping HID names to
        platform-specific human-readable names
      to_hid_table: Array mapping scancodes to HID keycodes
      from_hid_table: Array mapping HID keycodes to scancodes
    """

    def __init__(self, name, scancodes, displaynames, to_hid_table,
                 from_hid_table):
        self.name = name
        self.scancodes = scancodes
        self.displaynames = displaynames
        self.to_hid_table = to_hid_table
        self.from_hid_table = from_hid_table


def read_keytable(datadir, name, size, hid_names):
    """Read keycode tables.

    Arguments:
      datadir: Directory containing input data
      name: Platform name
    Returns:
      A Keytable object for the platform
    """
    with ReadFile(datadir, "{}_scancodes.csv".format(name)) as fp:
        scancodes = read_scancodes(fp)
    builder = KeymapBuilder([key for key in scancodes if key.code < size],
                            hid_names)
    with ReadFile(datadir, "{}_map.csv".format(name)) as fp:
        builder.apply_keymap(fp)
    with ReadFile(datadir, "{}_names.csv".format(name)) as fp:
        name_table = read_names(fp)
    displaynames = []
    for key in builder.keymap.values():
        displayname = name_table.pop(key.name, key.displayname)
        displaynames.append((key.code, displayname))
    if name_table:
        raise Error("Unused name mapping for {}".format(", ".join(
            sorted(name_table))))
    to_hid_table = [0] * size
    from_hid_table = [255] * 256
    for code, key in sorted(builder.keymap.items()):
        to_hid_table[code] = key.code
        if from_hid_table[key.code] == 255:
            from_hid_table[key.code] = code
    return Keytable(name, scancodes, displaynames, to_hid_table,
                    from_hid_table)


def read_all(datadir, hid_table):
    """Read all keycode tables.

    Arguments:
      datadir: Directory containing input data
    Returns:
      List of of Keytable objects
    """
    hid_names = {key.name: key for key in hid_table}
    platforms = [("linux", 256), ("macos", 128), ("windows", 256)]
    result = []
    for name, size in platforms:
        try:
            result.append(read_keytable(datadir, name, size, hid_names))
        except Error as ex:
            ex.platform = name
            raise
    return result
