/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-19 20:44:49
 * @description    :  以 POLL 方式监视 ev1
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/input.h>

int main() 
{
    int fd;
    struct pollfd pfd;
    pfd.events |= POLLIN;
    struct input_event ev_st;
    ssize_t len;

    fd = open("/dev/input/event1", O_RDONLY);
    if (fd < 0) 
    {
        perror("open");
        exit(1);    
    }

    pfd.fd = fd;
    poll(&pfd,1,-1);
    
    if(pfd.events & POLLIN)
    {
        len = read(fd, &ev_st, sizeof(struct input_event));
        if (len < 0)
        {
            perror("read");
            exit(1);
        }
        printf("type:0x%x, code:0x%x, value:0x%x\n", ev_st.type, ev_st.code, ev_st.value);
    }

    printf("poll done\n");
    exit(0);
    

}
