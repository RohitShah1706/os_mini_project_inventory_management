#include "../logger/log_generator.h"

void adminLog()
{
    int productsFd = open(PRODUCTS_FILENAME, O_RDWR);
    int adminLogFd = open(ADMIN_LOGS_FILENAME, O_CREAT | O_RDWR, 0666);

    int nProducts;
    read(productsFd, &nProducts, sizeof(nProducts));

    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Id", "Name", "Category", "Price", "Quantity", "IsDeleted");

    struct Product product;
    while (read(productsFd, &product, sizeof(product)))
    {
        // ! write product to admin log file in text format
        // ! use table format using fort.h

        int productId = product.productId;
        char productIdStr[100];
        sprintf(productIdStr, "%d", productId);
        char productName[100];
        strcpy(productName, product.name);

        char productCategory[100];
        strcpy(productCategory, product.category);

        int productQuantity = product.quantityAvailable;
        char productQuantityStr[100];
        sprintf(productQuantityStr, "%d", productQuantity);
        float productPrice = product.price;
        char productPriceStr[100];
        sprintf(productPriceStr, "%f", productPrice);
        char productIsDeleted[10];
        if (product.isDeleted)
        {
            strcpy(productIsDeleted, "true");
        }
        else
        {
            strcpy(productIsDeleted, "false");
        }

        ft_write_ln(table, productIdStr, productName, productCategory, productPriceStr, productQuantityStr, productIsDeleted);
    }
    const char *tableStr = ft_to_string(table);
    write(adminLogFd, tableStr, strlen(tableStr));
    close(productsFd);
    close(adminLogFd);
}

void clientLog(int userId, ft_table_t *table)
{
    char clientLogFilename[100];
    strcpy(clientLogFilename, "clientLogsFile");
    char cartIdStr[100];
    sprintf(cartIdStr, "%d", userId);
    strcat(clientLogFilename, cartIdStr);
    strcat(clientLogFilename, ".txt");
    int clientLogFd = open(clientLogFilename, O_CREAT | O_RDWR, 0666);

    const char *tableStr = ft_to_string(table);
    write(clientLogFd, tableStr, strlen(tableStr));
    close(clientLogFd);
}