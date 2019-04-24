# Table Generation Scripts

These scripts are incomplete, they are a new version of the scripts in the parent directory.

These scripts generate the keycode tables. You do not need to run these scripts yourself, because the outputs are checked in to the repository.

Running these scripts requires Python 3.

## Extracting Platform Keycodes

Various platforms define keycodes in header files in their SDK. The `extract.py` program extracts these definitions and puts them in CSV format. You don’t need to run this program yourself, the results are already available in the `data` directory. You can re-run this program if the SDK is updated with additional definitions (which does happen).

The program should automatically find the header files on Linux and macOS, if they are installed.

    python extract.py --platform=linux
    python extract.py --platform=macos

For Windows, you must specify the path to the `WinUser.h` header file in the Windows SDK.

    python extract.py --platform=windows --input=path/to/WinUser.h
