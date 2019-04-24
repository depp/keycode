#include "keytable.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    const char *name;
    unsigned code, out;
    int result;
    (void)argc;
    (void)argv;
    result = 0;
    for (code = 0; code < 256; code++) {
        name = keycode_to_id(code);
        if (name != NULL) {
            out = keycode_from_id(name);
            if (out != code) {
                fprintf(stderr,
                        "Error: keycode_to_id(0x%02x) = \"%s\", "
                        "keycode_from_id(\"%s\") = 0x%02x, expect 0x%02x\n",
                        code, name, name, out, code);
                result = 1;
            }
        }
    }
    name = keycode_to_id(-1);
    if (name != NULL) {
        fputs("Error: keycode_to_id(-1) != NULL\n", stderr);
        result = 1;
    }
    static const char *INVAL_NAMES[3] = {"xx", "",
                                         "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"};
    for (code = 0; code < 3; code++) {
        out = keycode_from_id(INVAL_NAMES[code]);
        if (out != 0) {
            fprintf(stderr,
                    "Error: keycode_from_id(\"%s\") = 0x%02x, expect 0\n",
                    INVAL_NAMES[code], out);
            result = 1;
        }
    }
    return result;
}
