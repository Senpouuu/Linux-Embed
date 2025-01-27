/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-27 15:03:02 
 * @description    :  key driver
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
static struct class *mkey_class; 
static struct drv_operations *mkey_drv_ops;


enum {led_OFF=0,led_ON,led_STATUS,key_STATUS};


int mkey_open(struct inode * inode, struct file * file)
{
    printk(KERN_ALERT "mkey_open\n");

    return 0;

}
ssize_t mkey_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    int ret;
    uint8_t mkey_buf;
    mkey_buf = mkey_drv_ops->ctl(key_STATUS, 0);
        
    // copy to user 
    ret = copy_to_user(buf, &mkey_buf, 1);
    
    return 1;

}
ssize_t mkey_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    char mkey_buf;
    int ret;
    printk(KERN_ALERT "mkey_write!\n");
    // copy from user 
    ret = copy_from_user(&mkey_buf,buf ,1); 
    // set mkey on or off
    if(mkey_buf == '0')
        mkey_drv_ops->ctl(led_OFF, 0);        
    else
        mkey_drv_ops->ctl(led_ON, 0);
    

    return 1;

}
unsigned int mkey_poll(struct file * file, struct poll_table_struct * pt)
{
    return 0;
}


static struct file_operations mkey_fops = {
    .owner = THIS_MODULE,
    .open = mkey_open,
    .read = mkey_read,
    .write = mkey_write,
    .poll = mkey_poll,  
};

static __init int mkey_init(void)
{
    struct device* dev;
    printk(KERN_ALERT "mkey_init\n");
    major = register_chrdev(0, "mkey", &mkey_fops);
    if(major < 0)
    {
        printk(KERN_ALERT "register_chrdev faimkey with %d\n", major);
        return major;
    }

    mkey_class = class_create(THIS_MODULE, "mkey_class");
    if (IS_ERR(mkey_class)) 
    {
        unregister_chrdev(major, "mkey_drv");
        return PTR_ERR(mkey_class);
    }

    dev = device_create(mkey_class, NULL, MKDEV(major, 0), NULL,"mkey");
    if(IS_ERR(dev))
    {
        class_destroy(mkey_class);
        unregister_chrdev(major, "mkey_drv");    
        return PTR_ERR(dev);
    }
    
    mkey_drv_ops = get_board_drv_opr();
    if(mkey_drv_ops == NULL)
    {
        device_destroy(mkey_class, MKDEV(major, 0));
        class_destroy(mkey_class);   
        unregister_chrdev(major, "mkey_drv");
        return PTR_ERR(mkey_drv_ops);
    }


    return 0;
}

static void __exit mkey_exit(void)
{
    printk(KERN_ALERT "mkey_exit\n");
    
    device_destroy(mkey_class, MKDEV(major, 0));
    class_destroy(mkey_class);   
    unregister_chrdev(major, "mkey_drv");

}


module_init(mkey_init);
module_exit(mkey_exit);
MODULE_LICENSE("GPL");



