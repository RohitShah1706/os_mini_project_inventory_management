#include "handler/server_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int main()
{
    int sockfd, clientSockFd;
    struct sockaddr_in serverAddress, clientAddress;
    pthread_t pthread;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NO);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error in creating socket\n");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Error in binding\n");
        exit(1);
    }

    int backlog = 5;
    int status = listen(sockfd, backlog);
    if (status < 0)
    {
        printf("Error in listening\n");
        exit(1);
    }
    printf("Listening on port: %d\n", PORT_NO);

    while (1)
    {
        int clientAddressLength = sizeof(clientAddress);
        // ! accept = blocking call
        clientSockFd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSockFd < 0)
        {
            printf("Error in accepting connection\n");
            exit(1);
        }
        printf("Connection accepted: %d\n", clientSockFd);
        // ! create a thread to handle the client connection
        if (pthread_create(&pthread, NULL, handleClientConnection, (void *)&clientSockFd) < 0)
        {
            perror("could not create thread\n");
            return 1;
        }
        // ! detach the thread
        // ! so that the resources are freed when the thread exits
        if (pthread_detach(pthread) < 0)
        {
            perror("could not detach thread\n");
            return 1;
        }
    }

    close(sockfd);
    return 0;
}