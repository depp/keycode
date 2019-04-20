all: x11_test

CFLAGS = -O2 -g
CWARN = -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Winit-self
ifdef WERROR
override CWARN += -Werror
endif
override CFLAGS := $(CWARN) $(CFLAGS)

keyid.o: keyid.c keyid.h
keytable_evdev.o: keytable_evdev.c
keytable_mac.o: keytable_mac.c
keytable_win.o: keytable_win.c
x11_test.o: x11_test.c keyid.h keytable.h

x11_test: LIBS += $(shell pkg-config --libs x11)
x11_test: x11_test.o keyid.o keytable_evdev.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
