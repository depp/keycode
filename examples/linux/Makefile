CFLAGS := -O2
CWARN := -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Winit-self -Wstrict-prototypes
CPPFLAGS = -I../../src
ifdef WERROR
override CWARN += -Werror
override WARN = 1
endif
ifdef WARN
override CFLAGS := $(CWARN) $(CFLAGS)
endif

all: x11_test
clean:
	rm -f x11_test.o x11_test

x11_test.o: x11_test.c ../../src/keytable.h
../../src/libkeycode.a:
	$(MAKE) -C ../../src libkeycode.a

x11_test: LIBS += $(shell pkg-config --libs x11)
x11_test: x11_test.o ../../src/libkeycode.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
