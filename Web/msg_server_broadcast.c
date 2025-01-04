/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 16:09:16
 * @description    :  板子上的UDP服务器程序，给Linux客户端广播信息
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
    int sfd,val;
    struct sockaddr_in server_addr;

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        exit(1);
    }

    val = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)) < 0)
    {
        perror("setsockopt1");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(BOARDCAST_IP);

    while(1)
    {
        printf("Enter message to broadcast: ");
        fgets(msg_buf, MAX_MSG_LEN, stdin);
        int len = strlen(msg_buf);
        if(len > 0 && msg_buf[len-1] == '\n')
            msg_buf[len-1] = '\0';

        if(sendto(sfd, msg_buf, len, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("sendto");
            exit(1);
        }

        printf("Message sent to %s:%d\n", BOARDCAST_IP, PORT);
    }

}