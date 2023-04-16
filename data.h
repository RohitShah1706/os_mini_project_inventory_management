#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define PORT_NO 5000
#define USERS_FILENAME "usersFile"
#define PRODUCTS_FILENAME "productsFile"
#define ORDERS_FILENAME "ordersFile"

typedef struct User
{
    int userId;
    char email[100];
    char password[100];
    int age;
    char phoneNo[15];
    char address[100];
    bool isAdmin;
} User;

struct Product
{
    int productId;
    char name[100];
    char category[100];
    int quantityAvailable;
    float price;
};

struct Order
{
    int orderId;
    int userId;
    int productId;
    int quantity;
    float price;
    char date[100];
};

#endif