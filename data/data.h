#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define PORT_NO 8080
#define USERS_FILENAME "usersFile"
#define PRODUCTS_FILENAME "productsFile"
#define CARTS_FILENAME "cartsFile"
#define ORDERS_FILENAME "ordersFile"
#define ADMIN_LOGS_FILENAME "adminLogsFile.txt"
#define PRODUCTS_TOTAL_ALLOWED 100

struct User
{
    int userId;
    char email[100];
    char password[100];
    int age;
    char phoneNo[15];
    char address[100];
    bool isAdmin;
};

struct Product
{
    int productId;
    char name[100];
    char category[100];
    int quantityAvailable;
    int isDeleted;
    float price;
};

struct Cart
{
    // ! each user has only one cart
    // ! so cartId = userId - handle while creating a new user
    int userId;
    // ! at max 10 products can be added to cart
    int productIds[10];
    int quantities[10];
    float prices[10];
    char productNames[10][100];
    int nProducts;
};

struct CartItem
{
    int productId;
    int quantity;
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

struct Message
{
    char data[100];
    int dataSize;
    int status;
};

#endif