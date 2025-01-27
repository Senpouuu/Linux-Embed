CC=arm-buildroot-linux-gnueabihf-gcc
KERN_DIR = /home/senpouuu/100ask_imx6ull_mini-sdk/Linux-4.9.88

led_template-y := board_6ull.o led_drv.o gpio_led.o
obj-m	+= led_template.o 

all: led_drv led_app

led_drv: led_drv.c
	make -C $(KERN_DIR) M=`pwd` modules

led_app: led_app.c
	$(CC) -o led_app led_app.c

clean:
	make -C $(KERN_DIR) M=`pwd` clean
	rm -f led_app

