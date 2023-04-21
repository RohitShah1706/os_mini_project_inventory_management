#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include "log_generator.h"
#include "data.h"
#include "messager.h"
#include "locker.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

int openFile(char *fileName, int flags);
void addProduct(int clientSocket, struct Product *product);
void handleUserMenu(int clientSocket, struct User *user);

#endif