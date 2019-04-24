/* Copyright 2011-2019 Dietrich Epp <depp@zdome.net>
   This file is licensed under the terms of the MIT license. See LICENSE.txt
   for details. */
#ifndef KEYCODE_KEYTABLE_H
#define KEYCODE_KEYTABLE_H
#ifdef __cplusplus
extern "C" {
#endif

enum {
    /* KEYCODE_NONE indicates that the platform-specific keycode does not exist.
       This appears in translation tables from HID keycodes to platform-specific
       keycodes. */
    KEYCODE_NONE = 255,
};

/*
 * =============================================================================
 * Linux
 * =============================================================================
 */

enum {
    /* NOTE: On Linux, the keycodes are offset by KEYCODE_EVDEV_OFFSET when they
       are sent by evdev, which is used by X11. */
    KEYCODE_EVDEV_OFFSET = 8
};

/* Get the HID keycode for the given Linux keycode. Returns KEY_None (0) if the
   Linux keycode is not mapped to an HID keycode. The Linux keycode can be taken
   from the "keycode" field in an XKeyEvent structure and subtracting
   KEYCODE_EVEV_OFFSET. Safe to call with any possible input. */
unsigned keycode_linux_to_hid(unsigned linux_keycode);

/* Get the display name for an HID key code on Linux. This returns "Super" as
   the name for the "GUI" keys. Returns NULL if the no Linux keycode is mapped
   to this HID keycode. Safe to call with any possible input. */
const char *keycode_linux_name(unsigned hid_keycode);

/* Get the raw name of the Linux key code. This is used for debugging. The
   resulting name is a constant in linux/input-event-codes.h, with the "KEY_"
   prefix removed. */
const char *keycode_linux_rawname(unsigned linux_keycode);

/* Mapping from Linux key codes to HID key codes.
   KEY_None (0) indicates no mapping. */
extern const unsigned char KEYCODE_LINUX_TO_HID[256];

/* Mapping from HID key codes to native key codes.
   KEYCODE_NONE (255) indicates no mapping. */
extern const unsigned char KEYCODE_LINUX_FROM_HID[256];

/*
 * =============================================================================
 * macOS
 * =============================================================================
 */

/* Get the HID keycode for the given macOS keycode. Returns KEY_None (0) if the
   macOS keycode is not mapped to an HID keycode. The macOS keycode can be taken
   from the "keycode" property of an NSEvent. Safe to call with any possible
   input. */
unsigned keycode_macos_to_hid(unsigned macos_keycode);

/* Get the raw name of the macOS key code. This is used for debugging. The
   resulting name is a constant in macos/input-event-codes.h, with the "KEY_"
   prefix removed. */
const char *keycode_macos_rawname(unsigned macos_keycode);

/* Get the display name for an HID key code on macOS. This returns "Command" as
   the name for the "GUI" keys. Returns NULL if the no macOS keycode is mapped
   to this HID keycode. Safe to call with any possible input. */
const char *keycode_macos_name(unsigned hid_keycode);

/* Mapping from macOS key codes to HID key codes.
   KEY_None (0) indicates no mapping. */
extern const unsigned char KEYCODE_MACOS_TO_HID[128];

/* Mapping from HID key codes to native key codes.
   KEYCODE_NONE (255) indicates no mapping. */
extern const unsigned char KEYCODE_MACOS_FROM_HID[256];

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
