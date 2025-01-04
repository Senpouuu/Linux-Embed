/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 16:52:01
 * @description    :  linux端的UDP客户端，接收多播信息
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
#define INTERFACE_IP "192.168.5.11"
#define MAX_MSG_LEN 1024

static char msg_buf[MAX_MSG_LEN];

int main()
{
    int sfd;
    socklen_t addrlen;
    struct sockaddr_in addr, mcast_addr;
    struct ip_mreqn mreq;

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        exit(1);
    }

    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_address.s_addr = inet_addr(INTERFACE_IP);
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_ifindex = if_nametoindex("ens37");
    if(setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, MULTICAST_IP, &addr.sin_addr);
    // if(bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    // {
    //     perror("bind");
    //     exit(1);
    // }
    
    
    addrlen = sizeof(mcast_addr);
    while(1)
    {
        int len = recvfrom(sfd, msg_buf, MAX_MSG_LEN, 0, (void*)&mcast_addr, &addrlen);
        if(len < 0)
        {
            perror("recvfrom");
            exit(1);
        }
        printf("Received message from %s:%d: %s\n", inet_ntoa(mcast_addr.sin_addr), ntohs(mcast_addr.sin_port), msg_buf);
        memset(msg_buf, 0, MAX_MSG_LEN);
    }

}
