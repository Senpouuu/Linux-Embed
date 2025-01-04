/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 16:09:16
 * @description    :  linux端的UDP客户端，接收广播信息
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
#define BOARDCAST_IP "255.255.255.255"
#define MAX_MSG_LEN 1024

static char msg_buf[MAX_MSG_LEN];


int main()
{
    int sfd,var;
    
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        exit(1);
    }

    var = 1;    
    if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &var, sizeof(var)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(BOARDCAST_IP);

    if(bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    while(1)
    {
        int len = recvfrom(sfd, msg_buf, MAX_MSG_LEN, 0, NULL, NULL);
        if(len < 0)
        {
            perror("recvfrom");
            exit(1);
        }
        printf("Received message: %s\n", msg_buf);
    }




}