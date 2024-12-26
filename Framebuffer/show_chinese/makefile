CC = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -finput-charset=UTF-8 -fexec-charset=GB2312

show_chn:show_chn.o
	$(CC) $(CFLAGS) $< -o $@    

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o show_chinse 