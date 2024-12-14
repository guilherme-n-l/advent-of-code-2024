CC = gcc
CFLAGS = -O0 -g
LDFLAGS = -lgsl -lgslcblas -lm

default: d13

d13: d13.c
	$(CC) d13.c $(CFLAGS) $(LDFLAGS) -o d13

clean:
	rm -f d13
