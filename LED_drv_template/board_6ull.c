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

#define GPIO_5_CLOCK_REG 0x020C406C
#define GPIO_5_MUX_REG 0x02290014
#define GPIO_5_GDIR_REG 0x020AC004
#define GPIO_5_DR_REG 0x020AC000

volatile static u_int32_t* gpio5_clock;
volatile static u_int32_t* gpio5_gdir;
volatile static u_int32_t* gpio5_mux;
volatile static u_int32_t* gpio5_dr;



int LED_init(int which)
{
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

// 0 LED off, 1 LED on , 2 LED Read,3 LED Map,4 LED Unmap
int LED_ctl(int which, char status)
{
    if(which == 0)
    {
        *gpio5_dr &= ~(1 << 3);
        *gpio5_dr |= (1 << 3);
        return 0;
    }

    if(which == 1)
    {
        *gpio5_dr &= ~(1 << 3);
        return 0;
    }    

    if(which == 2)
    {
        char led_buf,led_status;
        printk(KERN_ALERT "led_read\n");
        // read led status
        led_status = *gpio5_dr & (1 << 3);
        if(led_status)
            led_buf = '0';
        else
            led_buf = '1';
        return led_buf;
    }

    if(which == 3)
    {
        gpio5_clock = ioremap(GPIO_5_CLOCK_REG, 4);
        gpio5_gdir = ioremap(GPIO_5_GDIR_REG, 4);
        gpio5_mux = ioremap(GPIO_5_MUX_REG, 4);
        gpio5_dr = ioremap(GPIO_5_DR_REG, 4);
    }

    if(which == 4)
    {
        iounmap(gpio5_clock);
        iounmap(gpio5_gdir);
        iounmap(gpio5_mux);
        iounmap(gpio5_dr);
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
