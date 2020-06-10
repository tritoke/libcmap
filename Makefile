CFLAGS = -shared -fPIC -O3 -std=c99 -pedantic
CPPFLAGS = -D_DEFAULT_SOURCE

all: 
	gcc ${CFLAGS} ${CPPFLAGS} cmap.c -o libcmap.so

clean:
	rm -f libcmap.so

.PHONY: all clean
