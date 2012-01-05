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

Platform information
--------------------

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

The keyboard description can be retrieved at runtime by the
XkbGetKeyboard function, which returns an XkbDescRec structure with a
"names" and "geom" field.  Together these map key codes to four-byte
names.  Note that aliases in the "geom" field override those in the
"names" field.

More information: http://berrange.com/tags/scan-codes/

On Windows, Microsoft has specified the "Windows Virtual Key" for each
key, i.e., the key code.  As on the Mac, the hardware keycodes are
abstracted away by the operating system.  We can hard code names
corresponding to those constants.

How mapping works
-----------------

The game maintains two configuration tables: the keycode table and the
key id table.  Both map keys to actions.

The keycode table is derived from the key id table at runtime.  Since
the keycode table will vary from platform to platform, it is never
written to disk.  All configuration changes modify the key id table.

The key id table maps key identifiers, which are strings, to actions.
Sensible defaults can be hard-coded into the application without
worries about cross-platform concerns.  The table can be written to
and read from disk and shared across platforms.

Each platform has code which maps between key ids and key codes.  The
mapping is chosen so the same logical keys will map to the same ids on
most platforms.  For example, there is a key id "A" which maps to the
same key on all platforms, even if it produces the character "Q" with
French keyboard layouts.  Since some keyboards have unusual keys,
these are mapped to non-portable key names comprised of a platform
prefix and a key code, e.g., "MAC_192" or "WINDOWS_205".  These
non-portable keys will still be preserved in key id tables on
platforms which do not have these keys, but since there is no
corresponding key code, they will not show up in the key code table.
This is the reason why the key code table is always derived from the
key id table and never the other way around.

Platforms are also capable of mapping key codes to and from key
display names.  This allows the game to display to the user familiar
names for keys.  On a Mac with a French keyboard layout, key id "A"
will map to key code 0, and the game will display it as "Q" which
should match the label on the physical key.

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
