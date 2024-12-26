CC = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -lfreetype

show_font: show_font.o
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f *.o show_font