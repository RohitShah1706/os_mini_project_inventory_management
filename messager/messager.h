#ifndef MESSAGER_H
#define MESSAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void sendMessage(int sockfd, char *message);
void receiveMessage(int sockfd);

#endif