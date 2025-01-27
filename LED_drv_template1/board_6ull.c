/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-18 18:23:41
 * @description    :  用模板驱动框架编写的6ull板子的驱动程序，点灯。
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
#include <linux/io.h>

#include "drv_opr.h"
#include "gpio_led.h"



int LED_init(int which)
{
    gpio_led_init();
    return 0;
}

// 0 LED off, 1 LED on , 2 LED Read,3 LED Map,4 LED Unmap
int LED_ctl(int which, char status)
{
    if(which == 0)
    {
        gpio_led_set(0);
        return 0;
    }

    if(which == 1)
    {
        gpio_led_set(1);
        return 0;
    }    

    if(which == 2)
    {
        char led_buf,led_status;
        printk(KERN_ALERT "led_read\n");
        // read led status
        led_status = gpio_led_get(); 
        if(led_status)
            led_buf = '0';
        else
            led_buf = '1';
        return led_buf;
    }

    if(which == 3)
    {
        gpio_led_mmap();
        return 0;
    }

    if(which == 4)
    {
        gpio_led_munmap();
        return 0;
    }
    
    printk("led_ctl error\n");
    return -1;

}


struct drv_operations board_6ull_opr = {
    .init = LED_init,
    .ctl = LED_ctl
};

struct drv_operations *get_board_drv_opr(void)
{
    return &board_6ull_opr;
}
