/* Copyright 2011-2019 Dietrich Epp <depp@zdome.net>
   This file is licensed under the terms of the MIT license. See LICENSE.txt
   for details. */
#ifndef KEYCODE_KEYTABLE_H
#define KEYCODE_KEYTABLE_H
#ifdef __cplusplus
extern "C" {
#endif

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

/* Get the raw name of the Linux key code. This is used for debugging. The
   resulting name is a constant in linux/input-event-codes.h, with the "KEY_"
   prefix removed. */
const char *keycode_linux_rawname(unsigned linux_keycode);

/*
 * =============================================================================
 * macOS
 * =============================================================================
 */

/* Get the raw name of the macOS key code. This is used for debugging. The
   resulting name is a constant in macos/input-event-codes.h, with the "KEY_"
   prefix removed. */
const char *keycode_macos_rawname(unsigned macos_keycode);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
