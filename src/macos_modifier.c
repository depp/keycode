/* Copyright 2019 Dietrich Epp <depp@zdome.net>
   This file is licensed under the terms of the MIT license. See LICENSE.txt
   for details. */
#include "keytable.h"

/* For modifiers, we receive NSEventTypeFlagsChanged. This contains the keycode
   of the modifier which changed, but in order to tell the difference between
   key up and key down, we have to examine the modifier flags. This maps the
   modifiers to their associated flags.

   The flag values can be found in IOLLEvent.h. Note that we do not include the
   function key. This is by design. */
static const unsigned KEYCODE_MACOS_MODIFIERS[] = {
    0x00000010, /* Right Command: NX_DEVICERCMDKEYMASK */
    0x00000008, /* Left Command: NX_DEVICELCMDKEYMASK */
    0x00000002, /* Left Shift: NX_DEVICELSHIFTKEYMASK */
    0x00010000, /* Caps Lock: NX_ALPHASHIFTMASK */
    0x00000020, /* Left Option: NX_DEVICELALTKEYMASK */
    0x00000001, /* Left Control: NX_DEVICELCTLKEYMASK */
    0x00000004, /* Right Shift: NX_DEVICERSHIFTKEYMASK */
    0x00000040, /* Right Option: NX_DEVICERALTKEYMASK */
    0x00002000, /* Right Control NX_DEVICERCTLKEYMASK */
};

unsigned keycode_macos_modifier(unsigned macos_keycode) {
    if (54 <= macos_keycode && macos_keycode <= 62) {
        return KEYCODE_MACOS_MODIFIERS[macos_keycode - 54];
    }
    return 0;
}
