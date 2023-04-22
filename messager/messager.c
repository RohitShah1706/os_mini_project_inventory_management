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
    // for (int i = 0; i < sizeMsg; i++)
    // {
    //     printf("%c", message[i]);
    // }
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    char sizeMsgStr[1000];
    char newMessage[sizeMsg + 1];
    strcpy(newMessage, message);
    sprintf(sizeMsgStr, "%d", sizeMsg);
    ft_write_ln(table, "size", "message");
    ft_write_ln(table, sizeMsgStr, newMessage);
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}