/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 15:28:40
 * @description    :  板子上的TCP服务器程序，用于接收来自PC的TCP连接请求，接收来自PC的消息。
 *------------------------------------------------------------------------**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 1025
#define MAX_MSG_LEN 1024
#define SERVER_IP "192.168.5.9"

static char msg_buf[MAX_MSG_LEN]; 


int main()
{
    int sfd,nsfd,len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        exit(1);
    }

    if(bind(sfd, (void *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sfd, 5) < 0)
    {
        perror("listen");
        exit(1);
    }

    while(1)
    {
        len = sizeof(client_addr);
        nsfd = accept(sfd, (struct sockaddr*)&client_addr, &len);
        if(nsfd < 0)
        {
            perror("accept");
            exit(1);
        }
        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        while(1)
        {
            printf("Waiting for message...\n");
            int len = recv(nsfd, msg_buf, MAX_MSG_LEN, 0);
            if(len < 0)
            {
                perror("recv");
                exit(1);
            }
            else if(len == 0)
            {
                printf("Connection closed by client\n");
                break;
            }
            else
            {
                printf("Received message: %s\n", msg_buf);
                //do something with the message here
            }
        }
        close(nsfd);
        memset(msg_buf, 0, MAX_MSG_LEN);
    }
    

}