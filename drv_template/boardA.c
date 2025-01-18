/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-18 17:51:24
 * @description    :  板子A实现接口
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

int hello_init(int which)
{
    printk(KERN_INFO "Hello, world! This is boardA.\n");
    return 0;
}

int hello_ctl(int which, char status)
{
    printk(KERN_INFO "Hello, world! This is boardA.\n");
    return 0;
}


struct drv_operations boardA_opr = {
    .init = hello_init,
    .ctl = hello_ctl
};

struct drv_operations *get_board_drv_opr(void)
{
    return &boardA_opr;
}
