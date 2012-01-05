/* Copyright 2011-2012 Dietrich Epp <depp@zdome.net>
   See LICENSE.txt for details.  */
#ifndef KEYCODE_KEYTABLE_H
#define KEYCODE_KEYTABLE_H
#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned char MAC_HID_TO_NATIVE[256];
extern const unsigned char MAC_NATIVE_TO_HID[128];

extern const unsigned char EVDEV_HID_TO_NATIVE[256];
extern const unsigned char EVDEV_NATIVE_TO_HID[256];

extern const unsigned char WIN_HID_TO_NATIVE[256];
extern const unsigned char WIN_NATIVE_TO_HID[256];

#ifdef __cplusplus
}
#endif
#endif
