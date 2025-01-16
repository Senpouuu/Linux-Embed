/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-16 20:27:24
 * @description    :  led驱动,点亮一个LED灯
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

#define GPIO_5_CLOCK_REG 0x020C406C
#define GPIO_5_MUX_REG 0x02290014
#define GPIO_5_GDIR_REG 0x020AC004
#define GPIO_5_DR_REG 0x020AC000

static int major;
static struct class *led_class; 

volatile static u_int32_t* gpio5_clock;
volatile static u_int32_t* gpio5_gdir;
volatile static u_int32_t* gpio5_mux;
volatile static u_int32_t* gpio5_dr;

int led_open(struct inode * inode, struct file * file)
{
    printk(KERN_ALERT "led_open\n");
    // enable gpio clock
    *gpio5_clock &= ~((1 << 31) | (1 << 30));
    *gpio5_clock |= (1 << 31) | (1 << 30);
    
    // set mux to gpio pin
    *gpio5_mux &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
    *gpio5_mux |= (1 << 2) | (1 << 0);

    // set gpio to output mode
    *gpio5_gdir &= ~(1 << 3) ;
    *gpio5_gdir |= (1 << 3) ;

    return 0;

}
ssize_t led_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    char led_buf,led_status;
    int ret;
    printk(KERN_ALERT "led_read\n");
    // read led status

    led_status = *gpio5_dr & (1 << 3);
    if(led_status)
        led_buf = '0';
    else
        led_buf = '1';

    // copy to user 
    ret = copy_to_user(buf, &led_buf, 1);
    
    return 1;

}
ssize_t led_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    char led_buf;
    int ret;
    printk(KERN_ALERT "led_write\n");
    // copy from user 
    ret = copy_from_user(&led_buf,buf ,1); 
    // set led on or off
    if(led_buf)
        *gpio5_dr &= ~(1 << 3) ;

    if(led_buf == '0')
    {
        *gpio5_dr &= ~(1 << 3) ;
        *gpio5_dr |= (1 << 3) ;
    }
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

    gpio5_clock = ioremap(GPIO_5_CLOCK_REG, 4);
    gpio5_gdir = ioremap(GPIO_5_GDIR_REG, 4);
    gpio5_mux = ioremap(GPIO_5_MUX_REG, 4);
    gpio5_dr = ioremap(GPIO_5_DR_REG, 4);

    return 0;
}

static void __exit led_exit(void)
{
    printk(KERN_ALERT "led_exit\n");
    
    iounmap(gpio5_clock);
    iounmap(gpio5_gdir);
    iounmap(gpio5_mux);
    iounmap(gpio5_dr);

    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);   
    unregister_chrdev(major, "led_drv");

}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");



