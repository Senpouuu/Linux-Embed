/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-28 15:06:46
 * @description    :  串口回显实现,epoll超时设置
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
#include <sys/epoll.h>

int main()
{
    char buffer[100];
    struct termios tio;
    int ret;
    int fd;
    struct epoll_event event;
    int epfd;

    fd = open("/dev/ttymxc5",O_RDWR|O_NOCTTY);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

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

    epfd= epoll_create(1);
    if(ret < 0)
    {
        perror("epoll_create");
        exit(1);
    }
    event.data.fd = fd;
    event.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if(ret < 0)
    {
        perror("epoll_ctl");
        exit(1);
    }
    
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
        ret = epoll_wait(epfd,&event, 1, 1000);
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