#ifndef CLIENT_KEYBOARD_KEYID_H
#define CLIENT_KEYBOARD_KEYID_H
#ifdef __cplusplus
extern "C" {
#endif

/* Convert a key id to its HID code.  */
int keyid_code_from_name(const char *name);

/* Convert an HID code to its key id.  */
const char *keyid_name_from_code(int code);


#ifdef __cplusplus
}
#endif
#endif
