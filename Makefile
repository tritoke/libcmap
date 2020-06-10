CFLAGS = -O3 -flto -std=c99 -pedantic -Wall
CPPFLAGS = -D_DEFAULT_SOURCE

all: libcmap.a

libcmap.a: libcmap.o
	ar rc $@ $?
	ranlib $@

libcmap.o: cmap.h
	gcc -c cmap.c ${CFLAGS} ${CPPFLAGS} -o $@

clean:
	rm -f libcmap.a libcmap.o

.PHONY: all clean
