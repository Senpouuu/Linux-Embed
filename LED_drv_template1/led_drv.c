/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-18 18:24:09
 * @description    :  适配模板驱动框架
 *------------------------------------------------------------------------**/

#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
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


#include "drv_opr.h"

static int major;
static struct class *led_class; 
static struct drv_operations *led_drv_ops;


enum {LED_OFF=0,LED_ON,LED_STATUS,LED_MAP,LED_UNMAP};


int led_open(struct inode * inode, struct file * file)
{
    printk(KERN_ALERT "led_open\n");
    led_drv_ops->init(0);

    return 0;

}
ssize_t led_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    int ret;
    char led_buf;
    led_buf = led_drv_ops->ctl(LED_STATUS, 0);

    // copy to user 
    ret = copy_to_user(buf, &led_buf, 1);
    
    return 1;

}
ssize_t led_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    char led_buf;
    int ret;
    printk(KERN_ALERT "led_write!\n");
    // copy from user 
    ret = copy_from_user(&led_buf,buf ,1); 
    // set led on or off
    if(led_buf == '0')
        led_drv_ops->ctl(LED_OFF, 0);        
    else
        led_drv_ops->ctl(LED_ON, 0);
    

    return 1;

}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,  
};

static __init int led_init(void)
{
    struct device* dev;
    printk(KERN_ALERT "led_init\n");
    major = register_chrdev(0, "led", &led_fops);
    if(major < 0)
    {
        printk(KERN_ALERT "register_chrdev failed with %d\n", major);
        return major;
    }

    led_class = class_create(THIS_MODULE, "led_class");
    if (IS_ERR(led_class)) 
    {
        unregister_chrdev(major, "led_drv");
        return PTR_ERR(led_class);
    }

    dev = device_create(led_class, NULL, MKDEV(major, 0), NULL,"led");
    if(IS_ERR(dev))
    {
        class_destroy(led_class);
        unregister_chrdev(major, "led_drv");    
        return PTR_ERR(dev);
    }
    
    led_drv_ops = get_board_drv_opr();
    if(led_drv_ops == NULL)
    {
        device_destroy(led_class, MKDEV(major, 0));
        class_destroy(led_class);   
        unregister_chrdev(major, "led_drv");
        return PTR_ERR(led_drv_ops);
    }

    led_drv_ops->ctl(LED_MAP, 0);

    return 0;
}

static void __exit led_exit(void)
{
    printk(KERN_ALERT "led_exit\n");
    
    led_drv_ops->ctl(LED_UNMAP, 0);

    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);   
    unregister_chrdev(major, "led_drv");

}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");



