/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-27 14:58:00 
 * @description    :  6ull key resources 
 *------------------------------------------------------------------------**/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/uaccess.h> 
#include <linux/module.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#define GPIO_5_CLOCK_REG 0x020C406C

#define GPIO_5_3_MUX_REG 0x02290014
#define GPIO_5_1_MUX_REG 0x02290000

#define GPIO_5_GDIR_REG 0x020AC004
#define GPIO_5_DR_REG 0x020AC000


static struct resource led_resource[] = {
	[0] = {
		.start = GPIO_5_CLOCK_REG,
		.end   = GPIO_5_CLOCK_REG,
        .name  = "gpio5_clock",
		.flags = IORESOURCE_MEM,
		},
	[1] = {
		.start = GPIO_5_1_MUX_REG,
		.end   = GPIO_5_1_MUX_REG,
        .name  = "gpio5_key_mux",
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = GPIO_5_3_MUX_REG,
		.end   = GPIO_5_3_MUX_REG,
        .name  = "gpio5_led_mux",
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = GPIO_5_GDIR_REG,
		.end   = GPIO_5_GDIR_REG,
        .name  = "gpio5_gdir",
		.flags = IORESOURCE_MEM,
	},
	[4] = {
		.start = GPIO_5_DR_REG,
		.end   = GPIO_5_DR_REG,
        .name  = "gpio5_dr",
		.flags = IORESOURCE_MEM,
	}
};

void gpio5_release(struct device *dev)
{
    return;
}

static struct platform_device dev_gpio5_led = {
    .name = "gpio5_led",
    .num_resources = ARRAY_SIZE(led_resource),
    .resource = led_resource,
    .dev = {
        .release = gpio5_release,
    }
};



static __init int gpio_led_init(void)
{
    int err;
    err = platform_device_register(&dev_gpio5_led);

    return 0;
}  


static __exit void gpio_led_exit(void)
{
    platform_device_unregister(&dev_gpio5_led);
}


module_init(gpio_led_init);
module_exit(gpio_led_exit);

MODULE_LICENSE("GPL");