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
#include <poll.h>


int main(int argc, char *argv[])
{
    int fd;
    struct pollfd pfd;

    char key_stat = 0;
    fd = open("/dev/key_drv", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    pfd.fd = fd;
    pfd.events = POLLIN;

    while(1)
    {
        printf("waiting for key input...\n");
        poll(&pfd, 1, 1000);

        if(pfd.revents & POLLIN)    
        {
            key_stat = read(fd, &key_stat, 1);
            printf("key_stat = %d\n", key_stat);
        }
    }

    close(fd);
    return 0;
}
