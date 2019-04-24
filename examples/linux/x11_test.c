// Copyright 2018-2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.

// This will create a window and print out the X11 keycode and corresponding HID
// keycode and name for every key down event.
#include "keytable.h"

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void key_press(XKeyEvent *e) {
    printf("Key Press:\n");
    unsigned keycode = e->keycode - KEYCODE_EVDEV_OFFSET;
    const char *keyname = keycode_linux_rawname(keycode);
    if (keyname == NULL) {
        keyname = "unknown";
    }
    printf(
        "Key press:\n"
        "  X11 Keycode: 0x%02x (%s)\n"
        "\n",
        keycode, keyname);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "error: unable to open display '%s'\n",
                XDisplayName(NULL));
        exit(1);
    }

    int screen = DefaultScreen(dpy);
    int black_color = BlackPixel(dpy, screen),
        white_color = WhitePixel(dpy, screen);

    int width = 160, height = 120;
    Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 0, 0, width,
                                     height, 0, white_color, white_color);

    long event_mask = KeyPressMask | StructureNotifyMask | ExposureMask;
    XSelectInput(dpy, win, event_mask);
    XMapWindow(dpy, win);
    Atom wm_protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
    Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wm_delete_window, 1);
    GC gc = XCreateGC(dpy, win, 0, 0);
    XSetForeground(dpy, gc, black_color);

    while (1) {
        XEvent e;
        XNextEvent(dpy, &e);

        switch (e.type) {
        case Expose:
            XDrawLine(dpy, win, gc, 0, 0, width, height);
            XDrawLine(dpy, win, gc, 0, height, width, 0);
            XFlush(dpy);
            break;

        case ConfigureNotify:
            if (e.xconfigure.width != width || e.xconfigure.height != height) {
                width = e.xconfigure.width;
                height = e.xconfigure.height;
            }
            break;

        case ClientMessage:
            if (e.xclient.message_type == wm_protocols) {
                if (e.xclient.format == 32 &&
                    e.xclient.data.l[0] == (long)wm_delete_window) {
                    goto done;
                }
            }
            break;

        case KeyPress:
            key_press(&e.xkey);
            break;
        }
    }
done:

    XCloseDisplay(dpy);
    return 0;
}
