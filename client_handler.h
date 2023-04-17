#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "client_controller.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

char message[100];

void signUp(int sockfd);
bool login(int sockfd);
void handleServerConnection(int sockfd);

#endif