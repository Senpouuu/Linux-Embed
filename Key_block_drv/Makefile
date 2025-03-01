CC=arm-buildroot-linux-gnueabihf-gcc
KERN_DIR = /home/senpouuu/100ask_imx6ull_mini-sdk/Linux-4.9.88
obj-m	+= key_drv.o

all: key_drv key_app

key_drv: key_drv.c
	make -C $(KERN_DIR) M=`pwd` modules

key_app: key_app.c
	$(CC) -o key_app key_app.c

clean:
	make -C $(KERN_DIR) M=`pwd` clean
	rm -f key_app

