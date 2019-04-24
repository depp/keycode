# Changelog

## [2.0.0]

### Added

- Example programs for Linux, macOS, and Windows
- Helper functions which do bounds checking on translation table array accesses
- Modifier key helper function for macOS
- Key code decoder functions for Windows
- Translation functions from key codes to platform-specific “raw” names
- Build script for library file
- Added a test for identifier conversions

### Changed

- Reorganized source code
- Changed all function names and translation table names
- Windows key codes are now translated from lParam instead of wParam, since wParam depends on keyboard layout
- Generation scripts migrated to Python 3
- Table format changed to use CSV, redesigned
- Linux key codes are taken from header file instead of XKB configuration

## [1.0.0]

### Added

- Initial release

[2.0.0]: https://github.com/depp/keycode/compare/v1.0...v2.0.0
[1.0.0]: https://github.com/depp/keycode/releases/tag/v1.0
