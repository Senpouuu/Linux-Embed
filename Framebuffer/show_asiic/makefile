CC = arm-buildroot-linux-gnueabihf-gcc
CFLAGS = -lpthread

all: ldc_showchar


ldc_showchar: ldc_showchar.c
	$(CC) $(CFLAGS) -o ldc_showchar ldc_showchar.c

clean:
	rm -f ldc_showchar