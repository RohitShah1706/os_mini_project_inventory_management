#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "../router/server_router.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

char response[100];

bool checkLogin(struct User *user);
void doSignUp(int clientSocket, struct User *user);
void handleSignUp(int clientSocket);
bool handleLogin(int clientSocket, struct User *user);
void *handleClientConnection(void *arg);

#endif