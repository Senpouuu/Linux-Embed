/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-16 22:55:38
 * @description    :  点灯测试程序,on/off参数点灯
 *------------------------------------------------------------------------**/
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
    
    if(argc < 2)
    {
        printf("Usage: %s <on/off>\n", argv[0]);
        exit(1);
    }
    
    fd = open("/dev/led", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    if(strcmp(argv[1], "on"))
    {
        if(write(fd, "0", 1) != 1)
        {
            perror("write");
            exit(1);
        }
     
    }
    else if(strcmp(argv[1], "off"))
    {
        if(write(fd, "1", 1) != 1)
        {
            perror("write");
            exit(1);
        }
    }
    else
        printf("Invalid argument\n");


    close(fd);
    return 0;
}
