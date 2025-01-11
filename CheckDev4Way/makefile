CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=

all:getinfo slpweak select poll epoll signal sigaction
	
getinfo:getinfo.c
	$(CC) $(CFLAGS) $^ -o $@ 

slpweak:slpweak.c
	$(CC) $(CFLAGS) $^ -o $@ 

select:select.c
	$(CC) $(CFLAGS) $^ -o $@ 

poll:poll.c
	$(CC) $(CFLAGS) $^ -o $@ 

epoll:epoll.c
	$(CC) $(CFLAGS) $^ -o $@ 

signal:signal.c
	$(CC) $(CFLAGS) $^ -o $@ 
	
sigaction:sigaction.c
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f getinfo slpweak select poll epoll signal sigaction