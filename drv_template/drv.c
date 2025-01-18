/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-18 17:51:02
 * @description    :  驱动模板
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


#include "drv_opr.h"

#define DRV_NUM 2

static char kernel_buf[1024];
static int major = 0;

static struct class *hello_drv_class;
static struct drv_operations* drv_ops;

static int hello_drv_open(struct inode *inode, struct file *file)
{
    drv_ops->init(1);
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

static ssize_t hello_drv_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{   
    drv_ops->ctl(1,'R');
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    copy_to_user(buf, kernel_buf, 1024);
    return 1024;
}

static ssize_t hello_drv_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    drv_ops->ctl(1,'W');
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    copy_from_user(kernel_buf, buf, 1024);
    return 1024;
}


static int hello_drv_close(struct inode *inode, struct file *file)
{
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

static struct file_operations hello_drv_fops = {
    .owner = THIS_MODULE,
    .open = hello_drv_open, 
    .read = hello_drv_read, 
    .write = hello_drv_write, 
    .release = hello_drv_close
};


static int __init hello_drv_init(void)
{
    int i;
    int err;
    
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    major = register_chrdev(major, "hello_drv", &hello_drv_fops);
    hello_drv_class = class_create(THIS_MODULE, "hello_drv");
    err = PTR_ERR(hello_drv_class);
    if(IS_ERR(hello_drv_class))
    {
        unregister_chrdev(major, "hello_drv");
        return -1;
    }
    for(i = 0 ; i < DRV_NUM ; i++)
        device_create(hello_drv_class, NULL, MKDEV(major, i), NULL, "hello_drv%d", i);
    
    drv_ops = get_board_drv_opr();
    return 0;
}

static void __exit hello_drv_exit(void)
{
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    device_destroy(hello_drv_class, MKDEV(major, 0));
    class_destroy(hello_drv_class);
    unregister_chrdev(major, "hello_drv");
}

module_init(hello_drv_init);
module_exit(hello_drv_exit);

MODULE_LICENSE("GPL");
