/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2024-12-19 21:14:52
 * @description    :  以 epoll 方式监控ev1
 *------------------------------------------------------------------------**/


#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/select.h>
#include <sys/epoll.h>      
#include <linux/input.h>  


int main()  
{
    int fd;
    int epfd;
    ssize_t len;
    struct epoll_event ep_ev;
    struct input_event ev_st;

    // 创建 epoll 实例
    epfd = epoll_create1(0);
    if(epfd < 0) 
    {
        perror("epoll_create error");
        exit(1);
    }

    // 打开输入设备
    fd = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
    if(fd < 0) 
    {
        perror("open event1 error");
        exit(1);
    }

    // 设置 epoll 事件
    ep_ev.events = EPOLLIN; 
    ep_ev.data.fd = fd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ep_ev) < 0) 
    {
        perror("epoll_ctl error");
        exit(1);
    }


    epoll_wait(epfd, &ep_ev, 1, -1);
    len = read(fd, &ev_st, sizeof(struct input_event));
    if(len < 0) 
    {
        perror("read error");
        exit(1);
    }
    printf("Epoll fd: %d\n", ep_ev.data.fd);
    printf("type:0x%x, code:0x%x, value:0x%x\n", ev_st.type, ev_st.code, ev_st.value);

}