/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-28 15:06:46
 * @description    :  串口回显实现,select超时设置
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    char buffer[100];
    struct termios tio;
    int ret;
    int fd;
    fd_set rfds;
    struct timeval timeout;
 

    fd = open("/dev/ttymxc5",O_RDWR|O_NOCTTY);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);

    
    fcntl(fd,F_SETFL,FNDELAY);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }
    tcgetattr(fd,&tio);
    printf("input speed: %d\n", tio.c_ispeed);
    printf("output speed: %d\n", tio.c_ospeed);
    printf("Buard rate: %d\n", cfgetospeed(&tio));


    
    while(1)
    {
        scanf("%s",buffer);
        ret = write(fd,buffer,strlen(buffer));
        printf("Send: %s\n",buffer);
        if(ret < 0)
        {
            perror("write");
            exit(1);
        }

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        timeout.tv_sec = 1;
        ret = select(fd + 1, &rfds, NULL, NULL, &timeout);
        if(ret == 0)
        {
            printf("Timeout\n");
            continue;
        }

        ret = read(fd, buffer, 100);
        if (ret < 0)
        {
            perror("read");
            exit(1);
        }
        printf("Reply: %s\n", buffer);
    }

    exit(0);
}