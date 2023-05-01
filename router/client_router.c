#include "client_router.h"

void showUserMenu(int sockfd, struct User *user, bool *isLoggedIn, bool *isAdmin)
{
    int choice;
    while (1)
    {
        ft_table_t *table = ft_create_table();
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_write_ln(table, "Choice", "Choice Details");
        ft_write_ln(table, "0", "Logout");
        ft_write_ln(table, "1", "See user details");
        if (*isAdmin == false)
        {
            ft_write_ln(table, "2", "See all products");
            ft_write_ln(table, "3", "Add product to cart");
            ft_write_ln(table, "4", "See cart items");
            ft_write_ln(table, "5", "Update cart item");
            ft_write_ln(table, "6", "Place order");
        }
        if (*isAdmin == true)
        {
            ft_write_ln(table, "10", "Add a product");
            ft_write_ln(table, "11", "See all products");
            ft_write_ln(table, "12", "Update a product");
            ft_write_ln(table, "13", "Delete a product");
        }
        printf("%s\n", ft_to_string(table));
        ft_destroy_table(table);
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