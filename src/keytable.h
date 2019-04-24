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

/* Get the identifier for the give HID keycode. The identifier is a unique
   alphanumeric string that uniquely identifies the key. Returns NULL if the
   keycode does not exist or is not used in any mapping. */
const char *keycode_to_id(unsigned keycode);

/* Look up an HID keycode with by its identifier. Returns 0 if no keycode has
   the given identifier, or if the keycode is not used in any mapping. Lookup is
   case insensitive. */
unsigned keycode_from_id(const char *id);

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

/* Get the modifier flags for a given macOS keycode. Returns 0 if the keycode is
   not a modifier key. This can be used to distinguish key up and key down
   events for modifier keys, since modifier key events always have the event
   type "flagsChanged" and do not distinguish between key up and key down. */
unsigned keycode_macos_modifier(unsigned macos_keycode);

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

/*
 * =============================================================================
 * Windows
 * =============================================================================
 */

/* Get the HID keycode for the given Windows keycode. Returns KEY_None (0) if
   the Windows keycode is not mapped to an HID keycode. The Windows keycode can
   be taken from from the lParam of an event using keycode_windows_from_lparam.
   This function is safe to call with any possible input. */
unsigned keycode_windows_to_hid(unsigned windows_keycode);

/* Get the Windows keycode from the lparam passed to event handlers. This is
   equal to:

   return ((lparam >> 16) & 0x7f) | ((lparam & (1 << 24)) != 0 ? 0x80 : 0); */
unsigned keycode_windows_from_lparam(unsigned lparam);

/* Get the raw name of the Windows key code. This is used for debugging. The
   resulting name is a constant in Windows/input-event-codes.h, with the "KEY_"
   prefix removed. */
const char *keycode_windows_rawname(unsigned windows_keycode);

/* Get the display name for an HID key code on Windows. This returns "Command"
   as the name for the "GUI" keys. Returns NULL if the no Windows keycode is
   mapped to this HID keycode. Safe to call with any possible input. */
const char *keycode_windows_name(unsigned hid_keycode);

/* Mapping from Windows key codes to HID key codes.
   KEY_None (0) indicates no mapping. */
extern const unsigned char KEYCODE_WINDOWS_TO_HID[256];

/* Mapping from HID key codes to native key codes.
   KEYCODE_NONE (255) indicates no mapping. */
extern const unsigned char KEYCODE_WINDOWS_FROM_HID[256];

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
