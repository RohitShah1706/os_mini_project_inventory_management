#include "./server_router.h"

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
                fetchAllProducts(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 3:
            // ! add to cart
            if (isAdmin == false)
            {
                addToCart(clientSocket, user);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 4:
            // ! show cart
            if (isAdmin == false)
            {
                showCart(clientSocket, user);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 5:
            // ! update cart item - newQuantity = 0 means delete
            if (isAdmin == false)
            {
                updateCartItem(clientSocket, user);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 6:
            // ! checkout
            if (isAdmin == false)
            {
                checkout(clientSocket, user);
            }
            else
            {
                sendMessage(clientSocket, "Not a user");
            }
            break;
        case 10:
            // ! add product - only admin
            if (isAdmin == true)
            {
                struct Product product;
                if (read(clientSocket, &product, sizeof(product)) < 0)
                {
                    printf("Error in reading product\n");
                    exit(1);
                }
                addProduct(clientSocket, &product);
            }
            break;
        case 11:
            // ! show all products - only admin
            if (isAdmin == true)
            {
                fetchAllProducts(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 12:
            // ! update a product with id
            if (isAdmin == true)
            {
                updateProduct(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 13:
            // ! delete a product with id
            if (isAdmin == true)
            {
                deleteProduct(clientSocket);
            }
            else
            {
                sendMessage(clientSocket, "Not an admin");
            }
            break;
        case 0:
            // ! logout
            isLoggedIn = false;
            if (isAdmin == true)
            {
                // ! generate log
                adminLog();
            }
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