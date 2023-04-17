#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int handleFileCreation(char *fileName)
{
    int fd = open(fileName, O_RDWR);
    if (fd < 0)
    {
        fd = open(fileName, O_CREAT | O_RDWR, 0777);
        if (fd < 0)
        {
            printf("Error in creating %s file\n", fileName);
            exit(1);
        }
        // ! fd > 0 means we have to add some data to the file
        return fd;
    }
    // ! 0 = file already exists
    return 0;
}

void handleUsersFileCreation()
{
    int fd = handleFileCreation(USERS_FILENAME);
    if (fd > 0)
    {
        // ! create admin user
        struct User admin;
        admin.userId = 1;
        strcpy(admin.email, "admin");
        strcpy(admin.password, "password");
        admin.age = 20;
        strcpy(admin.phoneNo, "1234567890");
        strcpy(admin.address, "admin address");
        admin.isAdmin = true;
        int nUsers = 1;
        // ! write noOfUsers & admin user
        write(fd, &nUsers, sizeof(nUsers));
        write(fd, &admin, sizeof(admin));
    }
}

void handleProductsFileCreation()
{
    int fd = handleFileCreation(PRODUCTS_FILENAME);
    if (fd > 0)
    {
        // ! add initial number of products
        int nProducts = 0;
        write(fd, &nProducts, sizeof(nProducts));
    }
}

void handleOrdersFileCreation()
{
    int fd = handleFileCreation(ORDERS_FILENAME);
    if (fd > 0)
    {
        // ! add initial number of orders
        int nOrders = 0;
        write(fd, &nOrders, sizeof(nOrders));
    }
}

int main()
{
    handleUsersFileCreation();
    handleProductsFileCreation();
    handleOrdersFileCreation();
    return 0;
}