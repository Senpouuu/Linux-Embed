/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-3-6 19:47:37
 * @description    :  mmap drv
 *------------------------------------------------------------------------**/
#include "asm/memory.h"
#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/mm.h"
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
#include <linux/slab.h>
#include <asm/page.h>
#include <linux/fb.h>
#include <asm/page.h>


static int major;
static struct class *mmap_class; 
static char *mmap_buf;
static int mmap_buf_size = 1024 * 8;

int mmap_open(struct inode * inode, struct file * file)
{
    printk(KERN_ALERT "mmap_open\n");
    return 0;

}
ssize_t mmap_read(struct file * file, char __user * buf, size_t size, loff_t * ppos)
{
    
    printk(KERN_ALERT "mmap_read\n");
    copy_to_user(buf,mmap_buf, mmap_buf_size);

    return 1;

}
ssize_t mmap_write(struct file * file, const char __user * buf, size_t size, loff_t * ppos)
{
    printk(KERN_ALERT "mmap_write\n");
    copy_from_user(mmap_buf, buf, size);

    return 1;

}

int my_mmap(struct file * fp, struct vm_area_struct * vm)
{
    unsigned long phy = virt_to_phys(mmap_buf);
    vm->vm_page_prot = pgprot_writecombine(vm->vm_page_prot);

    if(remap_pfn_range(vm, vm->vm_start, phy >> PAGE_SHIFT, vm->vm_end - vm->vm_start, vm->vm_page_prot))
    {
        printk(KERN_ALERT "remap_pfn_range failed\n");
        return -ENOBUFS;
    }
    return 0;
}

static struct file_operations mmap_fops = {
    .owner = THIS_MODULE,
    .open = mmap_open,
    .read = mmap_read,
    .write = mmap_write,
    .mmap =  my_mmap
     
};

static __init int my_mmap_init(void)
{
    struct device* dev;
    printk(KERN_ALERT "mmap_init\n");
    major = register_chrdev(0, "mmap", &mmap_fops);
    if(major < 0)
    {
        printk(KERN_ALERT "register_chrdev faimmap with %d\n", major);
        return major;
    }

    mmap_class = class_create(THIS_MODULE, "mmap_class");
    if (IS_ERR(mmap_class)) 
    {
        unregister_chrdev(major, "mmap_drv");
        return PTR_ERR(mmap_class);
    }

    dev = device_create(mmap_class, NULL, MKDEV(major, 0), NULL,"m_mmap");
    if(IS_ERR(dev))
    {
        class_destroy(mmap_class);
        unregister_chrdev(major, "mmap_drv");    
        return PTR_ERR(dev);
    }

    mmap_buf = kmalloc(mmap_buf_size, GFP_KERNEL);
    if(!mmap_buf)
    {
        device_destroy(mmap_class, MKDEV(major, 0));
        class_destroy(mmap_class);
        unregister_chrdev(major, "mmap_drv");
        return -ENOMEM;
    }

    return 0;
}

static void __exit mmap_exit(void)
{
    printk(KERN_ALERT "mmap_exit\n");
    
    device_destroy(mmap_class, MKDEV(major, 0));
    class_destroy(mmap_class);   
    unregister_chrdev(major, "mmap_drv");
    kfree(mmap_buf);

}


module_init(my_mmap_init);
module_exit(mmap_exit);
MODULE_LICENSE("GPL");



