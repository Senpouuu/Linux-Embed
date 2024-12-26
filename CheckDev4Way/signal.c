/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-20 20:14:33
 * @description    :  在 poll 到之后发送信号通知主进程,来读取ev1的数据
 *------------------------------------------------------------------------**/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/input.h>

static int fd;
static struct input_event ev;

void signal_handler(int signum)
{
    printf("Received signal %d\n", signum);
    if(signum == SIGIO)
        read(fd, &ev, sizeof(struct input_event));
    
    printf("type:0x%x, code:0x%x, value:0x%x\n", ev.type, ev.code, ev.value);
}


int main()
{
    
    fd = open("/dev/input/event1", O_RDONLY);
    if (fd < 0) 
    {
        perror("open");
        exit(1);
    }
    fcntl(fd, F_SETOWN, getpid());
    /*使能“异步通知”               */
	int flag = fcntl(fd,F_GETFL);				
	fcntl(fd,F_SETFL,flag|FASYNC);				
														

    signal(SIGIO, signal_handler);
    
    while(1)
        pause();

    exit(0);
}