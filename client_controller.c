#include "client_controller.h"

void showUserDetails(struct User user)
{
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "User Detail", "Value");

    ft_write_ln(table, "Email", user.email);
    char ageStr[10];
    sprintf(ageStr, "%d", user.age);
    ft_write_ln(table, "Age", ageStr);
    ft_write_ln(table, "Phone number", user.phoneNo);
    ft_write_ln(table, "Address", user.address);
    if (user.isAdmin)
        ft_write_ln(table, "Is Admin", "Yes");
    else
        ft_write_ln(table, "Is Admin", "No");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
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
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Id", "Name", "Category", "Quantity", "Price");
    while (read(sockfd, &product, sizeof(product)) > 0)
    {
        if (product.productId == -1)
        {
            break;
        }
        char productIdStr[100];
        sprintf(productIdStr, "%d", product.productId);
        char quantityStr[100];
        sprintf(quantityStr, "%d", product.quantityAvailable);
        char priceStr[100];
        sprintf(priceStr, "%f", product.price);
        ft_write_ln(table, productIdStr, product.name, product.category, quantityStr, priceStr);
    }
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
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
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Field", "Value");
    char productIdStr[100];
    sprintf(productIdStr, "%d", product.productId);
    ft_write_ln(table, "Product id", productIdStr);
    if (strcmp(product.category, "") != 0)
        ft_write_ln(table, "Product category", product.category);
    if (product.quantityAvailable != -1)
    {
        char quantityStr[100];
        sprintf(quantityStr, "%d", product.quantityAvailable);
        ft_write_ln(table, "Product quantity", quantityStr);
    }
    if (product.price != -1)
    {
        char priceStr[100];
        sprintf(priceStr, "%f", product.price);
        ft_write_ln(table, "Product price", priceStr);
    }
    if (strcmp(product.name, "") != 0)
    {
        ft_write_ln(table, "Product name", product.name);
    }
    if (product.isDeleted == false)
    {
        ft_write_ln(table, "Product isDeleted", "False");
    }
    else
    {
        ft_write_ln(table, "Product isDeleted", "True");
    }
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

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
    if (cart.nProducts == 0)
    {
        printf("Cart is empty\n");
        return;
    }
    else
    {
        ft_table_t *table = ft_create_table();
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_write_ln(table, "Product id", "Quantity");
        for (int i = 0; i < cart.nProducts; i++)
        {
            char productIdStr[100];
            sprintf(productIdStr, "%d", cart.productIds[i]);
            char quantityStr[100];
            sprintf(quantityStr, "%d", cart.quantities[i]);
            ft_write_ln(table, productIdStr, quantityStr);
        }
        printf("%s\n", ft_to_string(table));
        ft_destroy_table(table);
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