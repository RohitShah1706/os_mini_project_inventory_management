#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "data.h"
#include "messager.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

char response[100];

int openFile(char *fileName, int flags);
bool checkLogin(struct User *user);
bool handleSignUp(int clientSocket);
bool handleLogin(int clientSocket, struct User *user);
void *handleClientConnection(void *arg);

#endif