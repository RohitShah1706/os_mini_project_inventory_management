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
    while (1)
    {
        int status = read(sockfd, &product, sizeof(product));
        if (status < 0)
        {
            printf("Error in reading product\n");
            exit(1);
        }
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
            write(sockfd, &choice, sizeof(choice));
            receiveMessage(sockfd);
            return;
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
        default:
            printf("Invalid choice\n");
            break;
        }
    }
}