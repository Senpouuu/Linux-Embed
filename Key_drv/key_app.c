/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-27 14:57:05 
 * @description    :  按键测试程序,读取按键状态,可以指定方式
 *------------------------------------------------------------------------**/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>


int main(int argc, char *argv[])
{
    int fd;
    uint8_t key_stat;
    
    if(argc < 2)
    {
        printf("Usage: %s <on/off>\n", argv[0]);
        exit(1);
    }
    
    fd = open("/dev/mkey", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    while (1) 
    {
        read(fd, &key_stat, 1);
        sleep(1);
        if(key_stat == 0)
        {
            if(write(fd, "1", 1) != 1)
            {
                perror("write");
                exit(1);
            }
        }
        else 
        {
            if(write(fd, "0", 1) != 1)
            {
                perror("write");
                exit(1);
            }
        }
    }

    close(fd);
    return 0;
}
