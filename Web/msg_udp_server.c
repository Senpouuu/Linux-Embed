/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-4 15:59:18
 * @description    :  板子上的UDP服务器程序，接收来自PC的消息。
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
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(SERVER_IP, &server_addr.sin_addr);

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd < 0) 
    {
        perror("socket");
        exit(1);
    }

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("bind");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1) 
    {
        int len = recvfrom(sfd, buf, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (len < 0) 
        {
            perror("recvfrom");
            exit(1);
        }
        printf("Received message from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Message: %s\n", buf);
    }

    close(sfd);


}