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
    // ! read nProducts
    lseek(fd, 0, SEEK_SET);
    int nProducts;
    read(fd, &nProducts, sizeof(nProducts));
    nProducts++;
    struct Product tempProduct;
    while (read(fd, &tempProduct, sizeof(tempProduct)) > 0)
    {
        if (strcmp(tempProduct.name, product->name) == 0 && tempProduct.isDeleted == false)
        {
            close(fd);
            return false;
        }
    }
    // ! write nProducts
    lseek(fd, 0, SEEK_SET);
    write(fd, &nProducts, sizeof(nProducts));
    // ! write product
    lseek(fd, 0, SEEK_END);
    product->productId = nProducts;
    product->isDeleted = false;
    write(fd, product, sizeof(*product));
    printf("Product added: %s\n", product->name);
    close(fd);
    return true;
}

void showAllProducts(int clientSocket)
{
    int fd = openFile(PRODUCTS_FILENAME, O_RDONLY);
    lseek(fd, 0, SEEK_SET);
    int nProducts;
    read(fd, &nProducts, sizeof(nProducts));
    struct Product product;
    printf("Sending all products now\n");
    while (read(fd, &product, sizeof(product)) > 0)
    {
        if (product.isDeleted == false)
        {
            printf("Product details: %s\n", product.name);
            if (write(clientSocket, &product, sizeof(product)) < 0)
            {
                printf("Error in sending product\n");
                close(fd);
                exit(1);
            }
        }
    }
    struct Product emptyProduct;
    emptyProduct.productId = -1;
    if (write(clientSocket, &emptyProduct, sizeof(emptyProduct)) < 0)
    {
        printf("Error in sending product\n");
        close(fd);
        exit(1);
    }
    close(fd);
    printf("All products sent\n");
}

bool updateProduct(int clientSocket)
{
    struct Product product;
    if (read(clientSocket, &product, sizeof(product)) < 0)
    {
        printf("Error in reading product\n");
        exit(1);
    }
    int fd = openFile(PRODUCTS_FILENAME, O_RDWR);
    lseek(fd, 0, SEEK_SET);
    int nProducts;
    read(fd, &nProducts, sizeof(nProducts));
    struct Product tempProduct;
    while (read(fd, &tempProduct, sizeof(tempProduct)) > 0)
    {
        if (tempProduct.productId == product.productId && tempProduct.isDeleted == false)
        {
            struct Product updatedProductDetails;
            if (strcmp(product.name, "") != 0)
            {
                strcpy(updatedProductDetails.name, product.name);
            }
            else
            {
                strcpy(updatedProductDetails.name, tempProduct.name);
            }
            if (strcmp(product.category, "") != 0)
            {
                strcpy(updatedProductDetails.category, product.category);
            }
            else
            {
                strcpy(updatedProductDetails.category, tempProduct.category);
            }
            if (product.price != -1)
            {
                updatedProductDetails.price = product.price;
            }
            else
            {
                updatedProductDetails.price = tempProduct.price;
            }
            if (product.quantityAvailable != -1)
            {
                updatedProductDetails.quantityAvailable = product.quantityAvailable;
            }
            else
            {
                updatedProductDetails.quantityAvailable = tempProduct.quantityAvailable;
            }
            lseek(fd, -sizeof(updatedProductDetails), SEEK_CUR);
            write(fd, &updatedProductDetails, sizeof(updatedProductDetails));
            close(fd);
            return true;
        }
    }
    close(fd);
    return false;
}

bool deleteProduct(int clientSocket)
{
    int productId;
    if (read(clientSocket, &productId, sizeof(productId)) < 0)
    {
        printf("Error in reading product id\n");
        exit(1);
    }
    int fd = openFile(PRODUCTS_FILENAME, O_RDWR);
    lseek(fd, 0, SEEK_SET);
    int nProducts;
    read(fd, &nProducts, sizeof(nProducts));
    struct Product tempProduct;
    while (read(fd, &tempProduct, sizeof(tempProduct)) > 0)
    {
        if (tempProduct.productId == productId && tempProduct.isDeleted == false)
        {
            tempProduct.isDeleted = true;
            lseek(fd, -sizeof(tempProduct), SEEK_CUR);
            write(fd, &tempProduct, sizeof(tempProduct));
            close(fd);
            return true;
        }
    }
    close(fd);
    return false;
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
        printf("Choice: %d\n", choice);
        switch (choice)
        {
        case 2:
            // ! show all products
            if (isAdmin == false)
            {
                showAllProducts(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 6:
            // ! add product - only admin
            if (isAdmin == true)
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
            if (isAdmin == true)
            {
                showAllProducts(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 8:
            // ! update a product with id
            if (isAdmin == true)
            {
                if (updateProduct(clientSocket))
                {
                    sendMessage(clientSocket, "Product updated successfully");
                }
                else
                {
                    sendMessage(clientSocket, "Error in updating product");
                }
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 9:
            // ! delete a product with id
            if (isAdmin == true)
            {
                if (deleteProduct(clientSocket))
                {
                    sendMessage(clientSocket, "Product deleted successfully");
                }
                else
                {
                    sendMessage(clientSocket, "Error in deleting product");
                }
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
            return;
        }
    }
    return;
}