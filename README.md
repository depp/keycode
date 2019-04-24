# Keycodes v2.0

The main problem here is that we want to specify a keyboard layout using key names, e.g., WASD, but this breaks as soon as you switch keyboard layouts. If you think WASD is reasonable on a US keyboard, you want to use ZQSD on a French keyboard. The solution is to respond to key inputs according to the location of the key, not the label (character) of the key.

Various input frameworks have botched this over the years. Developers often only use US layouts personally so test coverage is naturally poor. Reacting to keys by character codes causes problems more severe than a bad set of default layouts. For example, one commonly used guide to handling keyboard input on macOS would get confused by the shift key. For example, you could press “.” then, then press shift, and then release shift. The game will record the release of the “<” key instead, which is incorrect. SDL also has some severe problems when using different keyboard layouts, these have been fixed in 1.3.

The fundamental position of keys on keyboards is universal if the keys produce different characters and scan codes under different conditions. Therefore we can come up with a naming system for keys that is universally portable, at least to platforms for which we have reverse engineered the key codes.

## How to Use This Library

Copy the source files into your game (you only need _.c and _.h), or use a Git submodule if you'd rather. The source code license is the very permissive MIT license, so you do not have to share.

The “keycode.h” header defines symbolic names for keys that you can use, such as KEY_A and KEY_PageDown. The values correspond to the USB HID key codes from the USB HID Usage Tables, section 10. Each constant in this file defines the location of a key and not its character meaning. For example, KEY_A corresponds to A on a US keyboard, but it corresponds to Q on a French keyboard layout.

The HID Usage tables are available from the USB Implementers Forum website:

https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf

The “keytable.h” header defines various functions and constants for working with keycodes. These functions and constants let you look up the names of key codes, look up key codes by name, and translate between platform-specific key codes and portable HID key codes.

## Examples

Examples for Linux, macOS, and Windows are available in the “examples” directory.

## Table Generation

Scripts for generating the generated source code and data are available in the “scripts” directory.
