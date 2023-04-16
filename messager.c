#include "messager.h"

void sendMessage(int clientSocket, char *message)
{
    // ! first write the size of the message
    int sizeMsg = strlen(message);
    if (write(clientSocket, &sizeMsg, sizeof(sizeMsg)) < 0)
    {
        printf("Error in sending message size\n");
        exit(1);
    }
    printf("Size of message: %d\n", sizeMsg);
    // ! then write the message
    if (write(clientSocket, message, sizeMsg) < 0)
    {
        printf("Error in sending message\n");
        exit(1);
    }
}

void receiveMessage(int sockfd)
{
    // ! first read the size of the message
    int sizeMsg = 0;
    int status = read(sockfd, &sizeMsg, sizeof(sizeMsg));
    if (status < 0)
    {
        printf("Error in reading message\n");
        exit(1);
    }
    printf("Size of message: %d\n", sizeMsg);
    // ! then read the message
    char message[1024];
    status = read(sockfd, message, sizeMsg);
    if (status < 0)
    {
        printf("Error in reading message\n");
        exit(1);
    }
    printf("%s\n", message);
}