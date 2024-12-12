CC = gcc
CFLAGS = -O0 `pkg-config --cflags glib-2.0` -g -lm -Wall
LDFLAGS = `pkg-config --libs glib-2.0`

default: d11

d11: d11.c
	$(CC) d11.c $(CFLAGS) $(LDFLAGS) -o d11

clean:
	rm -f d11
