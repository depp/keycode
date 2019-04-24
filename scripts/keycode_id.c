/* Copyright 2019 Dietrich Epp.
   This file is licensed under the terms of the MIT license. See LICENSE.txt
   for details. */
unsigned keycode_from_id(const char *id) {
    unsigned char c, buf[KEYCODE_ID_MAXLEN];
    const char *p;
    int i, n, d;
    int left = 0, right = sizeof(KEYCODE_ID_ORDER), center;
    unsigned code;
    /* Normalize the ID to lower case. */
    for (n = 0; n < KEYCODE_ID_MAXLEN && id[n] != '\0'; n++) {
        c = id[n];
        if ('A' <= c && c <= 'Z') {
            c |= 32;
        }
        buf[n] = c;
    }
    if (id[n] != '\0') {
        return 0;
    }
    /* Binary search in sorted list of identifiers. */
    while (left < right) {
        center = left + (right - left) / 2;
        code = KEYCODE_ID_ORDER[center];
        p = KEYCODE_TO_ID_DATA + KEYCODE_TO_ID_OFFSET[code];
        for (i = 0;; i++) {
            if (i >= n) {
                if (p[i] != '\0') {
                    right = center;
                    break;
                }
                return code;
            }
            d = buf[i] - ((unsigned char)p[i] | 32);
            if (d < 0) {
                right = center;
                break;
            } else if (d > 0) {
                left = center + 1;
                break;
            }
        }
    }
    return 0;
}
