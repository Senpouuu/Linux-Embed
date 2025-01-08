CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=

oled_test: oled_test.c
	$(CC) $(CFLAGS) -o oled_test oled_test.c


at24c256: at24c256.c
	$(CC) $(CFLAGS) -o at24c256 at24c256.c

read_addr: read_addr.c
	$(CC) $(CFLAGS) -o read_addr read_addr.c

clean:
	rm -f oled_test at24c256