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
#include <signal.h>

void sig_term_handler(int signo)
{
    if(signo == SIGIO)
        printf("SIGIO received\n");
}


int main(int argc, char *argv[])
{
    int fd;
    struct pollfd pfd;
    char key_stat = 0;
    int oflags;

    fd = open("/dev/key_drv", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }
    
    if(fcntl(fd, F_SETOWN, getpid()) < 0) 
    {
        perror("fcntl");
        exit(1);
    }
    oflags = fcntl(fd, F_GETFL, 0);
    if(oflags < 0)
    {
        perror("fcntl");
        exit(1);
    }

    oflags |= FASYNC;
    if(fcntl(fd, F_SETFL, oflags) < 0)
    {
        perror("fcntl");
        exit(1);
    }
    
    signal(SIGIO,sig_term_handler);
    while(1); 
    close(fd);
    return 0;
}
