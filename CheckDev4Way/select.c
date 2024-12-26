/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-19 20:38:19
 * @description    :  以 Select 方式监控 ev1
 *------------------------------------------------------------------------**/

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/select.h>
#include <linux/input.h>  

int main()  
{
    int fd,nfds;
    ssize_t len;
    struct input_event ev_st;
    fd_set rfds;

    fd = open("/dev/input/event1", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    nfds = fd + 1;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    if(select(nfds, &rfds, NULL, NULL, NULL) > 0)
    {
        len = read(fd, &ev_st, sizeof(struct input_event));
        if(len < 0)
        {
            perror("read");
            exit(1);
        }
        printf("type:0x%x, code:0x%x, value:0x%x\n", ev_st.type, ev_st.code, ev_st.value);
    }

    printf("select end\n");
    exit(0); 

}