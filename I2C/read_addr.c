/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025年1月6日21:09:01
 * @description    :  读取一个某i2c设备的寄存器地址的值
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define DEV_ADDR 0x50 // 设备地址

int main(int argc, char *argv[])        
{
    if(argc < 2)
    {
        perror("Usage:./read_addr i2c_addr reg_addr ...");
        return -1;
    }

    int fd = open("/dev/i2c-0", O_RDWR); // 打开设备文件
    if(fd < 0)
    {   
        perror("Failed to open the i2c bus");
        return -1;
    }

    if(ioctl(fd, I2C_SLAVE, DEV_ADDR) < 0) // 设置设备地址
    {
        perror("Failed to set the i2c device address");
        return -1;
    }
    
    u_int8_t addr_h = strtoul(argv[1], NULL, 16);
    u_int8_t addr_l = strtoul(argv[2], NULL, 16);
    
    char cmd[2];
    cmd[0] = addr_h;cmd[1] = addr_l;
    

    
    if(write(fd, cmd, 2) < 2)
    {
        perror("Failed to write the i2c command");  
        return -1;
    }

    char byte;
    if(read(fd, &byte, 1) < 1)
    {
        perror("Failed to read the i2c response");
        return -1;
    }
    printf("The value of register 0x%x 0x%x\n", addr_h, addr_l);
    printf("value: 0x%x\n", byte);


    close(fd); // 关闭设备文件

}