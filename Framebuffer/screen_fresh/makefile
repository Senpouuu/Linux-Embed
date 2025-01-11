CC = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -lpthread

all:lcd

lcd:lcd.o
	$(CC) $(CFLAGS) $< -o $@    

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o lcd