/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-19 20:01:38
 * @description    :  以 阻塞/非阻塞 方式读取ev1
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>


int main(int argc, char *argv[])
{
    
    if(argc < 2)
    {
        printf("Usage: %s [NON_BLOCK]\n", argv[0]);
        exit(1);
    }
    int fd;
    struct input_event ev_st;
    
    if(strcmp(argv[1], "NON_BLOCK") == 0)
        fd = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);    
    else
        fd = open("/dev/input/event1", O_RDONLY);

    if(fd < 0)
    {
        perror("open");
        exit(1);    
    }

    while(1)
    {
        ssize_t n = read(fd, &ev_st, sizeof(struct input_event));
        if(n < 0)
        {
            if(errno == EAGAIN)
                continue;    
            else
            {
                perror("read");
                exit(1);
            }
        }
        printf("type:0x%x, code:0x%x, value:0x%x\n", ev_st.type, ev_st.code, ev_st.value);
    }
 
    exit(0);



}
