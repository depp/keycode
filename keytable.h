#ifndef CLIENT_KEYBOARD_KEYTABLE_H
#define CLIENT_KEYBOARD_KEYTABLE_H
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
