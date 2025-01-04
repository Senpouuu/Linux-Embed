/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 16:51:54
 * @description    :  板子上的UDP服务器程序，给Linux客户端发送信息
 *------------------------------------------------------------------------**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>


#define PORT 1025
#define MULTICAST_IP "239.255.255.250"
#define INTERFACE_IP "192.168.5.9"
#define MAX_MSG_LEN 1024

static char msg_buf[MAX_MSG_LEN];


int main()
{
    int sfd;
    struct sockaddr_in addr;
    struct ip_mreqn mreqn_if,mreq;
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd < 0)
    {
        perror("socket");   
        exit(1);
    }

    memset(&mreqn_if, 0, sizeof(mreqn_if));
    inet_pton(AF_INET, INTERFACE_IP, &mreqn_if.imr_address);   
    inet_pton(AF_INET, MULTICAST_IP, &mreqn_if.imr_multiaddr);
    mreqn_if.imr_ifindex = if_nametoindex("eth0");
    if(setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_IF, &mreqn_if, sizeof(mreqn_if)) < 0)
    {
        perror("setsockopt IP_MULTICAST_IF");
        exit(1);
    }

    memset(&mreq, 0, sizeof(mreq));
    inet_pton(AF_INET, MULTICAST_IP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, INTERFACE_IP, &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");
    if(setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt IP_ADD_MEMBERSHIP");
        exit(1);    
    }


    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, MULTICAST_IP, &addr.sin_addr);
    addr.sin_port = htons(PORT);

    while(1)
    {
        printf("Enter message to send: ");
        fgets(msg_buf, MAX_MSG_LEN, stdin);
        if(sendto(sfd, msg_buf, strlen(msg_buf), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            perror("sendto");
            exit(1);
        }
        memset(msg_buf, 0, MAX_MSG_LEN);
    }

    close(sfd);

    return 0;

}


