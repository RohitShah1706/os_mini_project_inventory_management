#include "client_handler.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main()
{
    int sockfd;
    struct sockaddr_in serverAddress;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error in creating socket\n");
        exit(1);
    }

    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NO);

    int status = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (status < 0)
    {
        printf("Error in connecting\n");
        exit(1);
    }
    printf("Connected to server\n");

    handleServerConnection(sockfd);
    return 0;
}