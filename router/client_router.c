#include "client_router.h"

void showUserMenu(int sockfd, struct User *user, bool *isLoggedIn, bool *isAdmin)
{
    int choice;
    while (1)
    {
        printf("0. Logout\n");
        printf("1. See user details\n");
        if (*isAdmin == false)
        {
            printf("2. See all products\n");
            printf("3. Add product to cart\n");
            printf("4. See cart items\n");
            printf("5. Update cart item\n");
            printf("6. Place order\n");
        }
        if (*isAdmin == true)
        {
            printf("10. Add a product\n");
            printf("11. See all products\n");
            printf("12. Update a product\n");
            printf("13. Delete a product\n");
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
        case 2:
            if (*isAdmin == false)
            {
                showAllProducts(sockfd);
                
            }
            else
            {
                printf("Not a user\n");
            }
            break;
        case 3:
            if (*isAdmin == false)
            {
                addProductToCart(sockfd);
            }
            else
            {
                printf("Not a user\n");
            }
            break;
        case 4:
            if (*isAdmin == false)
            {
                showCartItems(sockfd);
            }
            else
            {
                printf("Not a user\n");
            }
            break;
        case 5:
            if (*isAdmin == false)
            {
                updateCartItem(sockfd);
            }
            else
            {
                printf("Not a user\n");
            }
            break;
        case 6:
            if (*isAdmin == false)
            {
                placeOrder(sockfd);
            }
            else
            {
                printf("Not a user\n");
            }
            break;
        case 10:
            if (*isAdmin == true)
            {
                addProduct(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        case 11:
            if (*isAdmin == true)
            {
                showAllProducts(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        case 12:
            if (*isAdmin == true)
            {
                updateProduct(sockfd);
            }
            else
            {
                printf("Not an admin\n");
            }
            break;
        case 13:
            if (*isAdmin == true)
            {
                deleteProduct(sockfd);
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