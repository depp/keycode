# Copyright 2019 Dietrich Epp.
# This file is licensed under the terms of the MIT license. See LICENSE.txt
# for details.
"""Extract keycode tables from various SDK headers."""
import argparse
import csv
import os
import re
import sys

from common import Error


def linux_read_table(infile):
    """Read the Linux keymap from linux/input-event-codes.h."""
    if infile is None:
        infile = "/usr/include/linux/input-event-codes.h"
    with open(infile, "rb") as fp:
        text = fp.read()
    for m in re.finditer(rb"#define\s+KEY_(\w+)\s+(\w+)", text):
        if not m.group(2).startswith(b"KEY_"):
            yield int(m.group(2), 0), m.group(1).decode("ASCII")


def macos_open_header(infile):
    """Open the Carbon Events.h header file on macOS.

    Searches various SDK installation locations.
    """
    if infile is not None:
        return open(infile, "rb")
    # The headers may be in one of these places.
    bases = [
        "/",
        ("/Applications/Xcode.app/Contents/Developer/Platforms" +
         "/MacOSX.platform/Developer/SDKs/MacOSX.sdk"),
        "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
    ]
    for base in bases:
        fpath = os.path.join(
            base, "System/Library/Frameworks/Carbon.framework" +
            "/Frameworks/HIToolbox.framework/Headers/Events.h")
        try:
            return open(fpath, "rb")
        except FileNotFoundError:
            pass
    die("Could not find Carbon Events.h. Are the developer tools installed?")


def macos_read_table(infile):
    """Read the Linux keymap from Carbon Events.h."""
    with macos_open_header(infile) as fp:
        text = fp.read()
    for m in re.finditer(rb"kVK_(\w+)\s*=\s*(\w+)", text):
        yield int(m.group(2), 0), m.group(1).decode("ASCII")


def windows_read_table(infile):
    """Read the Windows keymap from WinUser.h."""
    if infile is None:
        die("Cannot find WinUser.h automatically, use --input.")
    with open(infile, "rb") as fp:
        text = fp.read()
    for m in re.finditer(rb"#define\s+VK_(\w+)\s+(\w+)", text):
        yield int(m.group(2), 0), m.group(1).decode("ASCII")


PLATFORMS = {
    "linux": linux_read_table,
    "macos": macos_read_table,
    "windows": windows_read_table,
}


def write_table(table, fp):
    """Write a keycode table to a file in CSV format."""
    writer = csv.writer(fp)
    writer.writerow(["Keycode", "Name"])
    for row in table:
        writer.writerow(row)


def extract(*, infile, outfile, platform, quiet=False):
    """Extract keycodes from a header file.

    Arguments:
      infile: Input file path, or None to automatically locate it
      outfile: Output file path, or None to automatically place it
      platform: Name of platform
    """
    read_table = PLATFORMS[platform]
    try:
        table = list(read_table(infile))
    except FileNotFoundError as ex:
        raise Error("Input file not found: {!r}".format(infile))
    if not table:
        raise Error(
            "Could not find keycode definitions in input file {!r}".format(
                infile))
        die("Could not find keycode definitions in input file.")
    if outfile is None:
        repo = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        outfile = os.path.join(repo, "data",
                               "{}_scancodes.csv".format(platform))
    if not quiet:
        print("Writing {}".format(outfile))
    with open(outfile, "w") as fp:
        write_table(table, fp)


def main(argv):
    p = argparse.ArgumentParser(
        description="Extract keycode tables from various SDK headers")
    p.add_argument("--platform",
                   help="extract tables for PLATFORM",
                   metavar="PLATFORM",
                   choices=PLATFORMS,
                   required=True)
    p.add_argument("--input", "-i", help="input header file")
    p.add_argument("--output", "-o", help="output CSV file")
    p.add_argument("--quiet",
                   "-q",
                   help="print no informational messages",
                   action="store_true")
    args = p.parse_args(argv)

    try:
        extract(infile=args.input,
                outfile=args.output,
                platform=args.platform,
                quiet=args.quiet)
    except Error as ex:
        print("Error:", ex, file=sys.stderr)
        raise SystemExit(1)


if __name__ == "__main__":
    main(sys.argv[1:])
