Keycodes
========

The main problem here is that users want to specify a keyboard layout
using key names, e.g., WASD, but it is better for computers to respond
to key presses using the key codes, e.g., 0x0D 0x01 0x02 0x03 (Mac).
The control layout should not be affected by keyboard layout, so what
is WASD on US keyboards should automatically become ZQSD on French
keyboards.

Various input frameworks have botched this over the years.  Developers
often only use US layouts personally so test coverage is naturally
poor.  Reacting to keys by character codes causes problems more severe
than a bad set of default layouts.  For example, the Omni Group
published a guide to handling input on the Mac that handled
punctuation very poorly.  Press "." then release while holding shift,
the game will record the release of the ">" key while the "." key is
still down.  SDL also has some severe problems when using different
keyboard layouts, these are fixed in 1.3 but that is little comfort
due to the sheer proliferation of SDL 1.2.  SDL 1.3 is also unreleased
as of this writing (Dec 2011) except as source code snapshots from
Mercurial with a conspicuous label that reads "WARNING: UNDER
CONSTRUCTION".

The fundamental position of keys on keyboards is universal if the keys
produce different characters and scan codes under different
conditions.  Therefore we can come up with a naming system for keys
that is universally portable, at least to platforms for which we have
reverse engineered the key codes.

How to use this library
-----------------------

Copy the source files into your game (you only need *.c and *.h), or
use a Git submodule if you'd rather.  The source code license is the
very permissive FreeBSD style license, so you do not have to share.

The "keycode.h" header defines symbolic names for keys that you can
use, such as KEY_A and KEY_PageDown.  The values correspond to the USB
HID key codes from the USB HID Usage Tables, section 10.  Each
constant in this file defines the location of a key and not its
character meaning.  For example, KEY_A corresponds to A on a US
keyboard, but it corresponds to Q on a French keyboard layout.

The HID Usage tables are available from the USB Implementers Forum
website:

http://www.usb.org/developers/hidpage/Hut1_12v2.pdf

The "keyid.h" header defines functions for looking up the name of a
key code and looking up a key code by name.  This allows you to write
configuration files in a sensible way.

The "keytable.h" header defines tables for converting between the HID
codes defined in "keycode.h" (e.g., KEY_A) and the platform-specific
codes.

Example of use
--------------

Suppose you are using WinAPI to make a game.  You will receive
WM_KEYDOWN and WM_KEYUP messages from the OS, and the associated
wParam will be a "Windows virtual key code".  Map the key code to a
cross-platform key code using the table in "keytable.h".

    #include "keytable.h"
    
    int VirtualKeyToKeyCode(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_SHIFT:
            wParam = MapVirtualKey((lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX);
            return wParam == VK_LSHIFT ? KEY_LeftShift : KEY_RightShift;
        case VK_CONTROL:
            return (lParam & 0x01000000) ? KEY_RightControl : KEY_LeftControl;
        case VK_MENU:
            return (lParam & 0x01000000) ? KEY_RightAlt : KEY_LeftAlt;
        default:
            return WIN_NATIVE_TO_HID[wParam];
        }
    }

    LRESULT CALLBACK wndProc(
        HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
        case WM_KEYDOWN:
            if (wParam >= 0 && wParam <= 255)
                handleKeyDown(VirtualKeyToKeyCode(wParam));
            break;

        ...
        }
    }

In your cross-platform game code, you can compare the key code against
the constants in "keycode.h".

    void handleKeyDown(int code)
    {
        /* WASD in the US, ZQSD in France... */
        switch (code) {
        case KEY_W: ...
        case KEY_A: ...
        case KEY_S: ...
        case KEY_D: ...
        }
    }

Implementation details
----------------------

There is one platform which is a pain: X11.

On Mac, keycodes are documented in the "Events.h" header file in the
Carbon HIToolbox framework.  They are constants prefixed with kVK_.
These appear to have been fixed for a very long time, possibly since
the 1980s.  Since there is no API for determining key names, we
instead can hard code names corresponding to the constants in
"Events.h".

On XOrg, there are a number of keyboard drivers which present
arbitrary keycodes to X.  Each driver may use different keycodes.  XKB
configuration files map the keycodes to 4-byte key names, then the
keyboard layout maps these names to keysyms.  The key names should be
reliable.  Note that it is theoretically possible for two keyboards
attached to an X server to generate different key codes for what are
logically the same keys.  We can hard code names corresponding to the
XKB key names.

We can query the keyboard description at runtime using the
XkbGetKeyboard function, but I have been unsuccessful in extracting
the key codes at runtime.  It appears that XKB eliminates this
information when the key map is compiled, and simply returns NULL at
runtime.

More information: http://berrange.com/tags/scan-codes/

On Windows, Microsoft has specified the "Windows Virtual Key" for each
key, i.e., the key code.  As on the Mac, the hardware keycodes are
abstracted away by the operating system.  We can hard code names
corresponding to those constants.

Table generation
----------------

Much of the code is generated from four files in the source tree, each
containing a table mapping USB HID usage codes to names.  The
'ids.txt' table maps HID codes to cross-platform key ids.  The other
files map HID codes to platform-specific key codes or key names.

The Mac table, 'mac.txt', referes to key names in "Events.h" which is
part of the Macintosh Carbon API.  An included script, "mac.py",
extracts key names from "Events.h" into a table, 'mac2.txt'.  The
'mac2.txt' table is checked into Git.

The Windows table, 'windows.txt', refers to windows virtual key codes
appearing in "Winuser.h".  As with the Mac, an included script,
"windows.py", extracts the "Winuser.h" data and creates 'windows2.txt'
mapping these key names to numeric values.

The XKB table, 'xkb.txt', refers to four-byte key names appearing in
XKB configuration files.  There is no fixed mapping from key codes to
key names in X11, so the key names are queried at runtime.
