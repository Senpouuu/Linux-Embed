/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025年1月6日20:42:03
 * @description    :  读写AT24C256的EEPROM
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define DEV_ADDR 0x50

int at24_byte_write(int fd,char addr_h, char addr_l, char data)
{
    char cmd[3] = {addr_h, addr_l, data};
    while(write(fd, cmd, 3) != 3)
        usleep(5000);    
    return 0;
}

int at24_page_write(int fd,char addr_h, char addr_l, char *data, int len)
{
    char cmd[len + 2];
    cmd[0] = addr_h; cmd[1] = addr_l;
    memcpy(cmd + 2, data, len);
    while(write(fd, cmd, len + 2) != len + 2)
        usleep(5000);       
    return 0;   
}

int at24_random_read(int fd,char addr_h, char addr_l, char *data, int len)
{
    char cmd[2] = {addr_h, addr_l};
    while(write(fd, cmd, 2) != 2)
        usleep(5000);

    while(read(fd, data, len)!= len)
        usleep(5000);
    return 0;   
}


int main()
{
    int fd = open("/dev/i2c-0", O_RDWR);
    if(fd < 0)
    {
        perror("open i2c-0 failed");
        exit(1);
    }
    if(ioctl(fd, I2C_SLAVE, DEV_ADDR) < 0)
    {
        perror("set i2c slave address failed");
        exit(1);
    }


    char data[16];
    at24_byte_write(fd,0x77,0x77,'A');
    at24_page_write(fd,0x00,0x00,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",16);
    
    at24_random_read(fd,0x77,0x77,data,16);
    printf("read:\n");
    for(int i = 0; i < 16; i++)
    {
        printf("0x%x\n", data[i]);
    }

    at24_random_read(fd,0x00,0x00,data,16);
    printf("read:\n");
    for(int i = 0; i < 16; i++)
    {
        printf("0x%x\n", data[i]);
    }


}