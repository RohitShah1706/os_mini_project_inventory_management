#include "client_controller.h"

void showUserDetails(struct User user)
{
    printf("User details: \n");
    printf("Email: %s\n", user.email);
    printf("Age: %d\n", user.age);
    printf("Phone number: %s\n", user.phoneNo);
    printf("Address: %s\n", user.address);
    printf("Is Admin: %d\n", user.isAdmin);
}

void addProduct(int sockfd)
{
    struct Product product;
    printf("Enter product name: ");
    scanf("%s", product.name);
    printf("Enter product category: ");
    scanf("%s", product.category);
    printf("Enter product quantity: ");
    scanf("%d", &product.quantityAvailable);
    printf("Enter product price: ");
    scanf("%f", &product.price);

    int status = write(sockfd, &product, sizeof(product));
    if (status < 0)
    {
        printf("Error in sending product\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void showAllProducts(int sockfd)
{
    struct Product product;
    printf("Getting all products now\n");
    while (read(sockfd, &product, sizeof(product)) > 0)
    {
        printf("Product id: %d\n", product.productId);
        if (product.productId == -1)
        {
            break;
        }
        printf("Product details: \n");
        printf("Product id: %d\n", product.productId);
        printf("Product name: %s\n", product.name);
        printf("Product category: %s\n", product.category);
        printf("Product quantity: %d\n", product.quantityAvailable);
        printf("Product price: %f\n", product.price);
        printf("\n");
    }
    printf("All products read\n");
}

void updateProduct(int sockfd)
{
    struct Product product;
    printf("Enter product id: ");
    scanf("%d", &product.productId);
    int toUpdate = 0;
    printf("Do you want to update product name? (1/0): ");
    scanf("%d", &toUpdate);
    if (toUpdate)
    {
        printf("Enter product name: ");
        scanf("%s", product.name);
    }
    else
    {
        strcpy(product.name, "");
    }
    printf("Do you want to update product category? (1/0): ");
    scanf("%d", &toUpdate);
    if (toUpdate)
    {
        printf("Enter product category: ");
        scanf("%s", product.category);
    }
    else
    {
        strcpy(product.category, "");
    }
    printf("Do you want to update product quantity? (1/0): ");
    scanf("%d", &toUpdate);
    if (toUpdate)
    {
        printf("Enter product quantity: ");
        scanf("%d", &product.quantityAvailable);
    }
    else
    {
        product.quantityAvailable = -1;
    }
    printf("Do you want to update product price? (1/0): ");
    scanf("%d", &toUpdate);
    if (toUpdate)
    {
        printf("Enter product price: ");
        scanf("%f", &product.price);
    }
    else
    {
        product.price = -1;
    }

    // print product details
    printf("Product details to be updated: \n");
    printf("Product id: %d\n", product.productId);
    printf("Product name: %s\n", product.name);
    printf("Product category: %s\n", product.category);
    printf("Product quantity: %d\n", product.quantityAvailable);
    printf("Product price: %f\n", product.price);
    printf("\n");

    int status = write(sockfd, &product, sizeof(product));
    if (status < 0)
    {
        printf("Error in sending product\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void showUserMenu(int sockfd, struct User *user, bool *isLoggedIn, bool *isAdmin)
{
    int choice;
    while (1)
    {
        printf("0. Logout\n");
        printf("1. See user details\n");
        if (isAdmin)
        {
            printf("6. Add a product\n");
            printf("7. See all products\n");
            printf("8. Update a product\n");
        }
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (write(sockfd, &choice, sizeof(choice)) < 0)
        {
            printf("Error in sending choice\n");
            exit(1);
        }
        switch (choice)
        {
        case 0:
            *isLoggedIn = false;
            *isAdmin = false;
            receiveMessage(sockfd);
            break;
        case 1:
            showUserDetails(*user);
            break;
        case 6:
            if (isAdmin)
            {
                addProduct(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        case 7:
            if (isAdmin)
            {
                showAllProducts(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        case 8:
            if (isAdmin)
            {
                updateProduct(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
        if (choice == 0)
        {
            return;
        }
    }
}