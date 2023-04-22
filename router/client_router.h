#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include "../controller/client_controller.h"
#include "../data/data.h"
#include "../messager/messager.h"
#include "../tableFormatters/fort.h"
#include "../logger/log_generator.h"

#include <stdio.h>
#include <stdbool.h>

void showUserDetails(struct User user);
void addProduct(int sockfd);
void showAllProducts(int sockfd);
void updateProduct(int sockfd);
void deleteProduct(int sockfd);
void addProductToCart(int sockfd);
void showCartItems(int sockfd);
void updateCartItem(int sockfd);
void placeOrder(int sockfd);
void showUserMenu(int sockfd, struct User *user, bool *isLoggedIn, bool *isAdmin);

#endif