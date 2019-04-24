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
        if len(row) != 2:
            raise error("Got {} columns, expected 2.".format(len(row)))
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


class Keytable:
    """A keycode table for a platform.

    Attributes:
      name: Platform name
      scancodes: List of Scancode objects
    """

    def __init__(self, name, scancodes):
        self.name = name
        self.scancodes = scancodes


def read_keytable(datadir, name):
    """Read keycode tables.

    Arguments:
      datadir: Directory containing input data
      name: Platform name
    Returns:
      A Keytable object for the platform
    """
    with ReadFile(datadir, "{}_scancodes.csv".format(name)) as fp:
        scancodes = read_scancodes(fp)
    return Keytable(name, scancodes)


def read_all(datadir):
    """Read all keycode tables.

    Arguments:
      datadir: Directory containing input data
    Returns:
      List of of Keytable objects
    """
    platforms = ["linux", "macos"]
    result = []
    for name in platforms:
        try:
            result.append(read_keytable(datadir, name))
        except Error as ex:
            ex.platform = name
            raise
    return result
