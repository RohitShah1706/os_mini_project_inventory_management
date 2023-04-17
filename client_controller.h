#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include "data.h"
#include "messager.h"

#include <stdio.h>
#include <stdbool.h>

void showUserDetails(struct User user);
void addProduct(int sockfd);
void showAllProducts(int sockfd);
void showUserMenu(int sockfd, struct User *user, bool *isLoggedIn, bool *isAdmin);

#endif