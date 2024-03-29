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
        printf("\n");
        exit(1);
    }
    printf("Product sent\n");
    printf("\n");
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
    ft_write_ln(table, "fields to be updated", "new value");
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
    if (product.isDeleted == 1 || product.quantityAvailable == 0)
    {
        ft_write_ln(table, "Product isDeleted", "True");
    }
    else
    {
        ft_write_ln(table, "Product isDeleted", "False");
    }
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    int status = write(sockfd, &product, sizeof(product));
    if (status < 0)
    {
        printf("Error in sending product\n");
        printf("\n");
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
        printf("\n");
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
        printf("\n");
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
        printf("\n");
        return;
    }
    else
    {
        ft_table_t *table = ft_create_table();
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_write_ln(table, "Id", "Name", "Price", "Quantity");
        for (int i = 0; i < cart.nProducts; i++)
        {
            char productIdStr[100];
            sprintf(productIdStr, "%d", cart.productIds[i]);

            char quantityStr[100];
            sprintf(quantityStr, "%d", cart.quantities[i]);

            char priceStr[100];
            sprintf(priceStr, "%f", cart.prices[i]);
            ft_write_ln(table, productIdStr, cart.productNames[i], priceStr, quantityStr);
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
        printf("\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

void placeOrder(int sockfd)
{
    struct Cart cart;
    read(sockfd, &cart, sizeof(cart));
    if (cart.nProducts == 0)
    {
        printf("Cart is empty. Add products to cart to place order.\n");
        printf("\n");
        // ! send 0 rs to server to indicate that we are not placing order
        float totalMoney = 0;
        int status = write(sockfd, &totalMoney, sizeof(totalMoney));
    }
    else
    {
        ft_table_t *table = ft_create_table();
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_write_ln(table, "Id", "Name", "Price", "Quantity", "Total");
        float totalPrice = 0.0;
        for (int i = 0; i < cart.nProducts; i++)
        {
            char productIdStr[100];
            sprintf(productIdStr, "%d", cart.productIds[i]);

            char quantityStr[100];
            sprintf(quantityStr, "%d", cart.quantities[i]);

            char priceStr[100];
            sprintf(priceStr, "%f", cart.prices[i]);

            float total = cart.prices[i] * cart.quantities[i];
            totalPrice += total;
            char totalStr[100];
            sprintf(totalStr, "%f", total);
            ft_write_ln(table, productIdStr, cart.productNames[i], priceStr, quantityStr, totalStr);
        }
        char totalPriceStr[100];
        sprintf(totalPriceStr, "%f", totalPrice);
        ft_write_ln(table, "", "", "", "Total", totalPriceStr);
        printf("%s\n", ft_to_string(table));
        clientLog(cart.userId, table);
        ft_destroy_table(table);

        // ! read message to find if locking was successful
        int sizeMsg = 0;
        int status = read(sockfd, &sizeMsg, sizeof(sizeMsg));
        if (status < 0)
        {
            printf("Error in reading message\n");
            exit(1);
        }
        printf("Size of message: %d\n", sizeMsg);
        // ! then read the message
        char message[1024];
        status = read(sockfd, message, sizeMsg);
        if (status < 0)
        {
            printf("Error in reading message\n");
            exit(1);
        }
        printf("Message received from server: %s\n", message);
        // ! check if locking was successful - by checking if message starts with "success"
        if (strncmp(message, "success", 7) != 0)
        {
            printf("Locking failed. Try again later.\n");
            return;
        }

        // ! take money from user
        float totalMoney = 0;
        printf("Enter total money to pay: ");
        scanf("%f", &totalMoney);
        status = write(sockfd, &totalMoney, sizeof(totalMoney));
        receiveMessage(sockfd);
    }
}