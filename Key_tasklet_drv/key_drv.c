/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-3-3 21:04:50
 * @description    :  key tasklet drv
 *------------------------------------------------------------------------**/


#include "asm/gpio.h"
#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/platform_device.h"
#include "linux/printk.h"
#include "linux/types.h"
#include "linux/wait.h"
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
#include <linux/wait.h>

static int major;
static struct class *key_class; 
static struct gpio_desc *key_gpio;
static int gpio_irq;
static char key_stat;

void key_tasklet_func(unsigned long data);

DECLARE_WAIT_QUEUE_HEAD(key_wq);
DECLARE_TASKLET(key_tasklet, key_tasklet_func, 0);

irqreturn_t key_irq_handler(int irq, void *dev_id) 
{
    tasklet_schedule(&key_tasklet);
    return IRQ_HANDLED;
}

void key_tasklet_func(unsigned long data)
{
    printk(KERN_ALERT "key tasklet handler\n");
    key_stat = gpiod_get_value(key_gpio);
    wake_up_interruptible(&key_wq);
}

int key_open(struct inode * inode, struct file * file)
{
    int err;

    err = gpiod_direction_input(key_gpio);
    if (err) 
    {
        printk(KERN_ALERT "Faikey to set key direction to input\n");
        return err;
    }
    printk(KERN_ALERT "key_open\n");

    key_stat = gpiod_get_value(key_gpio);
    return 0;

}

ssize_t key_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    printk(KERN_ALERT "key_read\n");
    if(file->f_flags & O_NONBLOCK)
        copy_to_user(buf, &key_stat, 0);

    else
    {
        wait_event_interruptible(key_wq, key_stat == 1);
        key_stat = 0;
    }
    return 1;
}  
ssize_t key_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    char key_buf;
    int err;
    printk(KERN_ALERT "key_write\n");
    err = copy_from_user(&key_buf,buf ,1); 
    
    if(key_buf == '0')
        gpiod_set_value(key_gpio, 0);
    else
        gpiod_set_value(key_gpio, 1); 

    return 1;
}

static struct file_operations key_fops = {
    .owner = THIS_MODULE,
    .open = key_open,
    .read = key_read,
    .write = key_write,  
};


int key_probe(struct platform_device * pdev)
{
    printk(KERN_ALERT "Probe key device\n");

    key_gpio = gpiod_get(&pdev->dev, "key",0);
    if (IS_ERR(key_gpio)) 
    {
        dev_err(&pdev->dev,"Faikey to get key gpio\n");
        return PTR_ERR(key_gpio);
    }

    gpio_irq = gpiod_to_irq(key_gpio);
    if(gpio_irq < 0)
    {
        printk(KERN_ALERT "Faikey to get key irq\n");
        return gpio_irq;
    }

    if (request_irq(gpio_irq, key_irq_handler, IRQF_TRIGGER_FALLING, "key_irq", NULL) < 0)
    {
        printk(KERN_ALERT "Faikey to request key irq\n");
        return -EBUSY;
    } 
    
    return 0;
}

int key_remove(struct platform_device * pdev)
{
    gpiod_put(key_gpio);
    return 0;
}

static const struct of_device_id of_key_table[] = {
	{ .compatible = "gpio5_pctrl,gpio5_led_pctrl"},
	{ /* sentinel */ }
};


static struct platform_driver key_platform_driver = {
    .probe = key_probe,
    .remove = key_remove,
    .driver = {
        .name = "gpio5_pctrl",
        .owner = THIS_MODULE,
        .of_match_table = of_key_table,
   }
};


static __init int mkey_init(void)
{
    struct device* dev;
    printk(KERN_ALERT "key_init\n");
    major = register_chrdev(0, "key_drv", &key_fops);
    platform_driver_register(&key_platform_driver);

    if(major < 0)
    {
        printk(KERN_ALERT "register_chrdev faikey with %d\n", major);
        return major;
    }

    key_class = class_create(THIS_MODULE, "key_class");
    if (IS_ERR(key_class)) 
    {
        gpiod_put(key_gpio);
        gpiod_put(key_gpio);
        platform_driver_unregister(&key_platform_driver);
        unregister_chrdev(major, "key_drv");
        return PTR_ERR(key_class);
    }

    dev = device_create(key_class, NULL, MKDEV(major, 0), NULL,"key_drv");
    if(IS_ERR(dev))
    {
        gpiod_put(key_gpio);
        gpiod_put(key_gpio);
        platform_driver_unregister(&key_platform_driver);
        class_destroy(key_class);
        unregister_chrdev(major, "key_drv");    
        return PTR_ERR(dev);
    }


    return 0;
}

static void __exit mkey_exit(void)
{
    printk(KERN_ALERT "key_exit\n");
    tasklet_kill(&key_tasklet);
    free_irq(gpio_irq, NULL);
    gpiod_put(key_gpio);
    platform_driver_unregister(&key_platform_driver);   
    device_destroy(key_class, MKDEV(major, 0));
    class_destroy(key_class);   
    unregister_chrdev(major, "key_drv");

}

module_init(mkey_init);
module_exit(mkey_exit);
MODULE_LICENSE("GPL");



