CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -ldl

all: dynamic

dynamic: dynamic.c library_first.so library_second.so
	$(CC) $(CFLAGS) dynamic.c -o dynamic $(LDFLAGS)

library_first.so: library_first.c library.h
	$(CC) $(CFLAGS) -shared -fPIC library_first.c -o library_first.so

library_second.so: library_second.c library.h
	$(CC) $(CFLAGS) -shared -fPIC library_second.c -o library_second.so

clean:
	rm -f dynamic library_first.so library_second.so

.PHONY: all clean
