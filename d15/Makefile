CC = gcc
CFLAGS = -O3 -g
LDFLAGS = 

default: d15

d15: d15.c
	$(CC) d15.c $(CFLAGS) $(LDFLAGS) -o d15

clean:
	rm -rf d15

run:
	make && ./d15

debug:
	make && gdb --tui ./d15
