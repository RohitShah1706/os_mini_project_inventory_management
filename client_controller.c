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
    while (read(sockfd, &product, sizeof(product)) > 0)
    {
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
    if (product.isDeleted == false)
    {
        printf("Product isDeleted: 0\n");
    }
    else
    {
        printf("Product isDeleted: 1\n");
    }
    printf("\n");

    int status = write(sockfd, &product, sizeof(product));
    if (status < 0)
    {
        printf("Error in sending product\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void deleteProduct(int sockfd)
{
    int productId;
    printf("Enter product id: ");
    scanf("%d", &productId);
    int status = write(sockfd, &productId, sizeof(productId));
    if (status < 0)
    {
        printf("Error in sending product id\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void addProductToCart(int sockfd)
{
    int productId, quantity;
    printf("Enter product id: ");
    scanf("%d", &productId);
    printf("Enter quantity: ");
    scanf("%d", &quantity);
    struct CartItem cartItem;
    cartItem.productId = productId;
    cartItem.quantity = quantity;
    int status = write(sockfd, &cartItem, sizeof(cartItem));
    if (status < 0)
    {
        printf("Error in sending product id\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void showCartItems(int sockfd)
{
    struct Cart cart;
    read(sockfd, &cart, sizeof(cart));
    printf("Cart details: \n");
    printf("Cart id: %d\n", cart.userId);
    printf("Cart items: \n");
    for (int i = 0; i < cart.nProducts; i++)
    {
        printf("Product id: %d\n", cart.productIds[i]);
        printf("Quantity: %d\n", cart.quantities[i]);
        printf("\n");
    }
}

void updateCartItem(int sockfd)
{
    int productId;
    printf("Enter product id: ");
    scanf("%d", &productId);
    int quantity;
    printf("Enter new quantity (New Quantity = 0 if you want to delete from cart): ");
    scanf("%d", &quantity);

    struct CartItem cartItem;
    cartItem.productId = productId;
    cartItem.quantity = quantity;
    int status = write(sockfd, &cartItem, sizeof(cartItem));
    if (status < 0)
    {
        printf("Error in sending product id\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void placeOrder(int sockfd)
{
    receiveMessage(sockfd);
}

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