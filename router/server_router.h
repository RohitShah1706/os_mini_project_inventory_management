#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include "../logger/log_generator.h"
#include "../data/data.h"
#include "../messager/messager.h"
#include "../locker/locker.h"
#include "../controller/server_controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

int openFile(char *fileName, int flags);
void addProduct(int clientSocket, struct Product *product);
void updateProduct(int clientSocket);
void fetchAllProducts(int clientSocket);
void deleteProduct(int clientSocket);
void addToCart(int clientSocket, struct User *user);
void showCart(int clientSocket, struct User *user);
void updateCartItem(int clientSocket, struct User *user);
void checkout(int clientSocket, struct User *user);
void handleUserMenu(int clientSocket, struct User *user);

#endif