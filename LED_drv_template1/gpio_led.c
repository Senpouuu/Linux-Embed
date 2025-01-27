/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-24 23:11:06
 * @description    :  将操作寄存器的部分从板子驱动中分离
 *------------------------------------------------------------------------**/

#include "linux/types.h"
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

#define GPIO_5_CLOCK_REG 0x020C406C
#define GPIO_5_MUX_REG 0x02290014
#define GPIO_5_GDIR_REG 0x020AC004
#define GPIO_5_DR_REG 0x020AC000

volatile static u_int32_t* gpio5_clock;
volatile static u_int32_t* gpio5_gdir;
volatile static u_int32_t* gpio5_mux;
volatile static u_int32_t* gpio5_dr;



void gpio_led_mmap(void)
{
    gpio5_clock = ioremap(GPIO_5_CLOCK_REG, 4);
    gpio5_gdir = ioremap(GPIO_5_GDIR_REG, 4);
    gpio5_mux = ioremap(GPIO_5_MUX_REG, 4);
    gpio5_dr = ioremap(GPIO_5_DR_REG, 4);
}

void gpio_led_munmap(void)
{
    iounmap(gpio5_clock);
    iounmap(gpio5_gdir);
    iounmap(gpio5_mux);
    iounmap(gpio5_dr);
}


void gpio_led_init(void)
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
}

void gpio_led_set(int value)
{
    if(value == 0)
    {
        *gpio5_dr &= ~(1 << 3);
        *gpio5_dr |= (1 << 3);
    }
    else
        *gpio5_dr &= ~(1 << 3);

}

char gpio_led_get(void)
{
    char led_status;
    led_status = *gpio5_dr & (1 << 3);
    return led_status;
}

void gpio_led_exit(void)
{
    gpio_led_munmap();
}
