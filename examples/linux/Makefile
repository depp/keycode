all: x11_test
clean:
	rm -f *.o x11_test

CFLAGS = -O2 -g
CWARN = -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Winit-self
ifdef WERROR
override CWARN += -Werror
endif
CPPFLAGS = -I../..

compile = $(CC) $(CPPFLAGS) $(CWARN) $(CFLAGS) -c $< -o $@

keyid.o: ../../keyid.c ../../keyid.h
	$(compile)
keytable_evdev.o: ../../keytable_evdev.c
	$(compile)
x11_test.o: x11_test.c ../../keyid.h ../../keytable.h
	$(compile)

x11_test: LIBS += $(shell pkg-config --libs x11)
x11_test: x11_test.o keyid.o keytable_evdev.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
