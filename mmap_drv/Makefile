CC=arm-buildroot-linux-gnueabihf-gcc
KERN_DIR = /home/senpouuu/100ask_imx6ull_mini-sdk/Linux-4.9.88
obj-m	+= mmap_drv.o

all: mmap_drv mmap_app

mmap_drv: mmap_drv.c
	make -C $(KERN_DIR) M=`pwd` modules

mmap_app: mmap_app.c
	$(CC) -o mmap_app mmap_app.c

clean:
	make -C $(KERN_DIR) M=`pwd` clean
	rm -f mmap_app

