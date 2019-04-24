CFLAGS := -O2 -std=c90
CWARN := -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Winit-self -Wstrict-prototypes
ifdef WERROR
override CWARN += -Werror
override WARN = 1
endif
ifdef WARN
override CFLAGS := $(CWARN) $(CFLAGS)
endif

all: libkeycode.a

objs := \
	keycode_id.o \
	linux_fromhid.o linux_name.o linux_rawname.o linux_tohid.o

clean:
	rm -f $(objs) libkeycode.a

keycode_id.o: keycode_id.c keytable.h
linux_fromhid.o: linux_fromhid.c keytable.h
linux_rawname.o: linux_rawname.c keytable.h
linux_name.o: linux_name.c keytable.h
linux_tohid.o: linux_tohid.c keytable.h

libkeycode.a: $(objs)
	$(AR) rcsD $@ $^

.PHONY: all clean
