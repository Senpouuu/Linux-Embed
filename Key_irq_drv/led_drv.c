/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-2-27 21:50:36 
 * @description    :  key irq ctl led on / off
 *------------------------------------------------------------------------**/


#include "asm/gpio.h"
#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/platform_device.h"
#include "linux/printk.h"
#include "linux/types.h"
#include <linux/io.h>
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
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>


static int major;
static struct class *led_class; 
static struct gpio_desc *led_gpio;
static struct gpio_desc *key_gpio;
static int gpio_irq;

irqreturn_t key_irq_handler(int irq, void *dev_id) 
{
    int led_stat;
    printk(KERN_ALERT "led irq handler\n");
    led_stat = gpiod_get_value(led_gpio);
    // 处理中断的代码
    if (irq == gpio_irq) 
    {
        if(led_stat == 0)
            gpiod_set_value(led_gpio, 1);
        else
            gpiod_set_value(led_gpio, 0);

        return IRQ_HANDLED; 
    } 
    else 
    {
        return IRQ_NONE;
    }
}


int led_open(struct inode * inode, struct file * file)
{
    int err;

    err = gpiod_direction_output(led_gpio, 1);
    if (err) 
    {
        printk(KERN_ALERT "Failed to set LED direction to output\n");
        return err;
    }
    printk(KERN_ALERT "led_open\n");
    
    return 0;

}


ssize_t led_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    char led_status;
    printk(KERN_ALERT "led_read\n");
    led_status = gpiod_get_value(led_gpio);
    copy_to_user(buf, &led_status, 0);
    return 1;
}  
ssize_t led_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    char led_buf;
    int err;
    printk(KERN_ALERT "led_write\n");
    err = copy_from_user(&led_buf,buf ,1); 
    
    if(led_buf == '0')
        gpiod_set_value(led_gpio, 0);
    else
        gpiod_set_value(led_gpio, 1); 

    return 1;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,  
};


int led_probe(struct platform_device * pdev)
{
    printk(KERN_ALERT "Probe led device\n");

    led_gpio = gpiod_get(&pdev->dev, "led",0);
    if (IS_ERR(led_gpio)) 
    {
        dev_err(&pdev->dev,"Failed to get led gpio\n");
        return PTR_ERR(led_gpio);
    }
    gpiod_direction_output(led_gpio, 1);

    key_gpio = gpiod_get(&pdev->dev, "key",0);
    if (IS_ERR(key_gpio)) 
    {
        dev_err(&pdev->dev,"Failed to get key gpio\n");
        return PTR_ERR(key_gpio);
    }
    gpiod_direction_input(key_gpio);

    gpio_irq = gpiod_to_irq(key_gpio);
    if(gpio_irq < 0)
    {
        printk(KERN_ALERT "Failed to get led irq\n");
        return gpio_irq;
    }

    if (request_irq(gpio_irq, key_irq_handler, IRQF_TRIGGER_FALLING, "led_irq", NULL) < 0)
    {
        printk(KERN_ALERT "Failed to request led irq\n");
        return -EBUSY;
    } 
    
    return 0;
}

int led_remove(struct platform_device * pdev)
{
    gpiod_put(led_gpio);
    return 0;
}

static const struct of_device_id of_led_table[] = {
	{ .compatible = "gpio5_pctrl,gpio5_led_pctrl"},
	{ /* sentinel */ }
};


static struct platform_driver led_platform_driver = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "gpio5_pctrl",
        .owner = THIS_MODULE,
        .of_match_table = of_led_table,
   }
};


static __init int led_init(void)
{
    struct device* dev;
    printk(KERN_ALERT "led_init\n");
    major = register_chrdev(0, "led", &led_fops);
    platform_driver_register(&led_platform_driver);

    if(major < 0)
    {
        printk(KERN_ALERT "register_chrdev failed with %d\n", major);
        return major;
    }

    led_class = class_create(THIS_MODULE, "led_class");
    if (IS_ERR(led_class)) 
    {
        gpiod_put(led_gpio);
        gpiod_put(key_gpio);
        platform_driver_unregister(&led_platform_driver);
        unregister_chrdev(major, "led_drv");
        return PTR_ERR(led_class);
    }

    dev = device_create(led_class, NULL, MKDEV(major, 0), NULL,"led");
    if(IS_ERR(dev))
    {
        gpiod_put(led_gpio);
        gpiod_put(key_gpio);
        platform_driver_unregister(&led_platform_driver);
        class_destroy(led_class);
        unregister_chrdev(major, "led_drv");    
        return PTR_ERR(dev);
    }


    return 0;
}

static void __exit led_exit(void)
{
    printk(KERN_ALERT "led_exit\n");
    free_irq(gpio_irq, NULL);
    gpiod_put(led_gpio);
    gpiod_put(key_gpio);
    platform_driver_unregister(&led_platform_driver);   
    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);   
    unregister_chrdev(major, "led_drv");

}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");



