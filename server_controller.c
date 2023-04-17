#include "server_controller.h"

int openFile(char *fileName, int flags)
{
    int fd = open(fileName, flags);
    if (fd < 0)
    {
        printf("Error in opening file %s\n", fileName);
        exit(1);
    }
    return fd;
}

bool addProduct(struct Product *product)
{
    // TODO: check if product already exists
    // TODO: if first product, set product id to 1
    printf("Adding product: %s\n", product->name);
    int fd = openFile(PRODUCTS_FILENAME, O_RDWR);
    lseek(fd, 0, SEEK_SET);
    struct Product tempProduct;
    while (read(fd, &tempProduct, sizeof(tempProduct)) > 0)
    {
        if (strcmp(tempProduct.name, product->name) == 0)
        {
            return false;
        }
    }
    lseek(fd, 0, SEEK_END);
    product->productId = tempProduct.productId + 1;
    write(fd, product, sizeof(*product));
    printf("Product added: %s\n", product->name);
    return true;
}

void showAllProducts(int clientSocket)
{
    int fd = openFile(PRODUCTS_FILENAME, O_RDONLY);
    lseek(fd, 0, SEEK_SET);
    struct Product product;
    while (read(fd, &product, sizeof(product)) > 0)
    {
        if (write(clientSocket, &product, sizeof(product)) < 0)
        {
            printf("Error in sending product\n");
            exit(1);
        }
    }
    struct Product emptyProduct;
    emptyProduct.productId = -1;
    if (write(clientSocket, &emptyProduct, sizeof(emptyProduct)) < 0)
    {
        printf("Error in sending product\n");
        exit(1);
    }
}

void handleUserMenu(int clientSocket, struct User *user)
{
    int choice;
    bool isLoggedIn = true;
    bool isAdmin = user->isAdmin;
    printf("User logged in: %s\n", user->email);
    while (isLoggedIn)
    {
        if (read(clientSocket, &choice, sizeof(choice)) < 0)
        {
            printf("Error in reading choice\n");
            exit(1);
        }
        switch (choice)
        {
        case 6:
            // ! add product - only admin
            if (isAdmin)
            {
                struct Product product;
                if (read(clientSocket, &product, sizeof(product)) < 0)
                {
                    printf("Error in reading product\n");
                    exit(1);
                }
                if (!addProduct(&product))
                {
                    sendMessage(clientSocket, "Error in adding product");
                }
                else
                {
                    sendMessage(clientSocket, "Product added successfully");
                }
            }
            break;
        case 7:
            // ! show all products - only admin
            if (isAdmin)
            {
                showAllProducts(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 0:
            // ! logout
            isLoggedIn = false;
            break;
        default:
            break;
        }
        if (choice == 0)
        {
            sendMessage(clientSocket, "Logged out successfully");
        }
    }
    return;
}