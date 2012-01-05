/* Copyright 2011-2012 Dietrich Epp <depp@zdome.net>
   See LICENSE.txt for details.  */
#ifndef KEYCODE_KEYID_H
#define KEYCODE_KEYID_H
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
