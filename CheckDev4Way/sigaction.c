#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/input.h>

static struct input_event ev;
static int fd;

void sig_action_handler(int signum,siginfo_t *act,void *arg)
{
    read(fd,&ev,sizeof(struct input_event));    
    printf("type:0x%x, code:0x%x, value:0x%x\n", ev.type, ev.code, ev.value);
}

int main()
{
    int err;
    struct sigaction sa;

    sa.sa_flags = 1;
    sa.sa_sigaction = sig_action_handler;
    sa.sa_restorer = NULL;
    sigemptyset(&sa.sa_mask);
    
    sigset_t sig;
    sigfillset(&sig);
    sigdelset(&sig,SIGIO);
    sigprocmask(SIG_SETMASK,&sig,NULL);

    fd = open("/dev/input/event1", O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    if(sigaction(SIGIO,&sa,NULL) < 0)
    {
        perror("sigaction");
        exit(1);
    }

    err = fcntl(fd,F_SETOWN,getpid());
    err = fcntl(fd,F_GETFL);
    err = fcntl(fd,F_SETFL,err | O_ASYNC);
    if(err < 0)
    {
        perror("fcntl");
        exit(1);
    }
    
    while(1)
        pause();

    exit(0);

}
