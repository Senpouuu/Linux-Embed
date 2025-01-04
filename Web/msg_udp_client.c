/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 15:52:47
 * @description    :  linux端的UDP客户端，用于连接服务器并发送消息
 *------------------------------------------------------------------------**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1025
#define MAX_MSG_LEN 1024
#define SERVER_IP "192.168.5.9"


static char buf[MAX_MSG_LEN];

int main()
{
    int sfd;
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd < 0)
    {
        perror("socket error");
        exit(1);
    }

    //bind();

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while(1)
    {
        printf("input message: "); 
        fgets(buf,MAX_MSG_LEN,stdin);
        printf("send message: %s\n",buf);   

        if(sendto(sfd,buf,MAX_MSG_LEN,0,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
        {
            perror("sendto error");
            exit(1);
        }
    }

}
