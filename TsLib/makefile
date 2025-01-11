CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=-lts -lm

all: read_ts

read_ts: read_ts.c
	$(CC) $(CFLAGS) -o read_ts read_ts.c

clean:
	rm -f read_ts
