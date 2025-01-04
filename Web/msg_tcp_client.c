/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 15:11:20
 * @description    :  linux端的TCP客户端，用于连接服务器并发送消息
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.5.9"
#define PORT 1025
#define MSG_LEN 1024

static char msg_buf[1024];

int main()
{
    int sfd;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;

    if(sfd < 0)
    {
        perror("socket error");
        exit(1);
    }
    //bind()
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);
    
    
    if(connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect error");
        exit(1);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);

    while(1)
    {
        printf("input message:");
        fgets(msg_buf, MSG_LEN, stdin);
        send(sfd, msg_buf, strlen(msg_buf), 0);
        memset(msg_buf, 0, MSG_LEN);
    }


}