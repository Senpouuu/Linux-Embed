CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=-lpthread -lts


all : drawLCD

drawLCD: drawLCD.c
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -f drawLCD



