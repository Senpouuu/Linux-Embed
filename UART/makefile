CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=

all: loopback_select loopback_poll loopback_epoll

loopback_select: loopback_select.c
	$(CC) $(CFLAGS) -o loopback_select loopback_select.c
	
loopback_poll: loopback_poll.c
	$(CC) $(CFLAGS) -o loopback_poll loopback_poll.c

loopback_epoll: loopback_epoll.c
	$(CC) $(CFLAGS) -o loopback_epoll loopback_epoll.c

clean:
	rm -f loopback_epoll loopback_poll loopback_select

