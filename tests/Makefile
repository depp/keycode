CFLAGS := -O2
CWARN := -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wmissing-prototypes -Winit-self -Wstrict-prototypes
CPPFLAGS = -I../src
ifdef WERROR
override CWARN += -Werror
override WARN = 1
endif
ifdef WARN
override CFLAGS := $(CWARN) $(CFLAGS)
endif

all: id_test
clean:
	rm -f id_test.o id_test

id_test.o: id_test.c ../src/keytable.h
../src/libkeycode.a:
	$(MAKE) -C ../src libkeycode.a

id_test: LIBS += $(shell pkg-config --libs x11)
id_test: id_test.o ../src/libkeycode.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
