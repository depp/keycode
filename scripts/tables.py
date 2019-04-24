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

VALID_NAME = re.compile(r"[a-z0-9]*(?: [a-z0-9]+)*", re.IGNORECASE)
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
