/**========================================================================
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-26 16:13:59
 * @description    :  6ull gpio5 dts driver
 *========================================================================**/

#include "linux/platform_device.h"
#include "linux/printk.h"
#include "linux/types.h"
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
#include <linux/of.h>


#include "drv_opr.h"

static volatile int32_t *gpio5_clock;
static volatile int32_t *gpio5_mux;
static volatile int32_t *gpio5_gdir;
static volatile int32_t *gpio5_dr;


int drv_gpio5_probe(struct platform_device *pdev)
{
    int err;
    struct device_node *np;
    
    int32_t addr;
    printk("probe gpio5 led driver");
    np = pdev->dev.of_node;
    if(np == NULL)
        return -ENODEV;
    
    err = of_property_read_u32(np, "gclock", &addr);
    if(err)
        return -EINVAL;
    else
    {
        gpio5_clock = ioremap(addr, 4);
        printk("clk remapped!");
        // enable gpio clock
        *gpio5_clock &= ~((1 << 31) | (1 << 30));
        *gpio5_clock |= (1 << 31) | (1 << 30);
    }

    err = of_property_read_u32(np, "mux", &addr);
    if(err) 
        return -EINVAL;
    else
    {
        gpio5_mux = ioremap(addr, 4);
        printk("mux remapped!");
        // set mux to gpio pin
        *gpio5_mux &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
        *gpio5_mux |= (1 << 2) | (1 << 0);
    }


    err = of_property_read_u32(np, "gdir", &addr);
    if(err)
        return -EINVAL;
    else
    {
        gpio5_gdir = ioremap(addr, 4);
        printk("gdir remapped!");
        // set gpio to output mode
        *gpio5_gdir &= ~(1 << 3) ;
        *gpio5_gdir |= (1 << 3) ;
    }

    err = of_property_read_u32(np, "dr", &addr);
    if(err)
        return -EINVAL;
    else
        gpio5_dr = ioremap(addr, 4);
    printk("dr remapped!");

    return 0;
}

int LED_ctl(int which, char status)
{
    if(which == 0)
    {
        *gpio5_dr &= ~(1 << 3);
        *gpio5_dr |= (1 << 3);
        return 0;
    }

    if(which == 1)
    {
        *gpio5_dr &= ~(1 << 3);
        return 0;
    }    

    if(which == 2)
    {
        char led_buf,led_status;
        printk(KERN_ALERT "led_read\n");
        // read led status
        led_status = *gpio5_dr & (1 << 3);
        if(led_status)
            led_buf = '0';
        else
            led_buf = '1';
        return led_buf;
    }
    
    printk("led_ctl error\n");
    return -1;

}

int drv_gpio5_remove(struct platform_device *pdev)
{
    iounmap(gpio5_clock);
    iounmap(gpio5_mux);
    iounmap(gpio5_gdir);
    iounmap(gpio5_dr);
    return 0;
}

static const struct of_device_id of_led_table[] = {
	{ .compatible = "gpio5,gpio5_led"},
	{ /* sentinel */ }
};


static struct platform_driver drv_gpio5 = {
    .remove = drv_gpio5_remove,
    .probe = drv_gpio5_probe,
    .driver = {
        .name = "gpio5_led",
        .owner = THIS_MODULE,
        .of_match_table = of_led_table,
    }
};

static int __init drv_gpio5_init(void)
{
    int err;
    err = platform_driver_register(&drv_gpio5);
    return 0;
}

static void __exit drv_gpio5_exit(void)
{
    platform_driver_unregister(&drv_gpio5);
}

static struct drv_operations led_drv_opr = {
   .init = NULL,
   .ctl = LED_ctl,
};

struct drv_operations *get_board_drv_opr(void)
{
    return &led_drv_opr;
}

EXPORT_SYMBOL(get_board_drv_opr);

module_init(drv_gpio5_init);
module_exit(drv_gpio5_exit);

MODULE_LICENSE("GPL");