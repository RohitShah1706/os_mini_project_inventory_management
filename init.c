#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

union semun
{
    int val;
    struct semid_ds *buf;  // ! buffer for IPC_STAT, IPC_SET
    unsigned short *array; // ! array for GETALL, SETALL
};

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

void handleCartsFileCreation()
{
    int fd = handleFileCreation(CARTS_FILENAME);
    if (fd > 0)
    {
        // ! add initial number of carts
        int nCarts = 0;
        write(fd, &nCarts, sizeof(nCarts));
        // ! write cart for admin user - even if there are no products in the cart
        struct Cart cart;
        cart.userId = 1;
        cart.nProducts = 0;
        write(fd, &cart, sizeof(cart));
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

void handleSemaphoreCreation()
{
    key_t key;
    int semid;

    key = ftok(".", 'a');
    if (key < 0)
    {
        perror("ftok");
        exit(1);
    }
    int totalSemaphores = PRODUCTS_TOTAL_ALLOWED;
    semid = semget(key, totalSemaphores, 0666 | IPC_CREAT);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }
    // ! initialize all semaphores to 1
    union semun semArg;
    semArg.val = 1; // ! binary semaphore
    for (int i = 0; i < totalSemaphores; i++)
    {
        int semaphoreToSelect = i;
        if (semctl(semid, semaphoreToSelect, SETVAL, semArg) < 0)
        {
            perror("semctl");
            exit(1);
        }
    }
}

int main()
{
    handleUsersFileCreation();
    handleProductsFileCreation();
    handleCartsFileCreation();
    handleOrdersFileCreation();
    handleSemaphoreCreation();
    return 0;
}