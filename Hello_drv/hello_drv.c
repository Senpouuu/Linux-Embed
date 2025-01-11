/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-11 18:22:43
 * @description    :  Hello驱动实现
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

static char kernel_buf[1024];
static int major = 0;

static struct class *hello_drv_class;

static int hello_drv_open(struct inode *inode, struct file *file)
{
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

static ssize_t hello_drv_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{    
    printk("%s,%s line %d\n",__FILE__,__FUNCTION__,__LINE__);
    copy_to_user(buf, kernel_buf, 1024);
    return 1024;
}

static ssize_t hello_drv_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
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

    device_create(hello_drv_class, NULL, MKDEV(major, 0), NULL, "hello_drv");
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
