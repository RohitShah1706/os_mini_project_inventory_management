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

void addProduct(int clientSocket, struct Product *product)
{
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
            sendMessage(clientSocket, "Product already exists");
            return;
        }
    }
    // ! lock record
    if (lockFileRecord(fd, nProducts) == false)
    {
        close(fd);
        sendMessage(clientSocket, "Error locking file");
        return;
    }
    // ! write nProducts
    lseek(fd, 0, SEEK_SET);
    write(fd, &nProducts, sizeof(nProducts));
    // ! write product
    lseek(fd, 0, SEEK_END);
    product->productId = nProducts;
    product->isDeleted = false;
    write(fd, product, sizeof(*product));
    close(fd);
    // ! unlock record
    if (unlockFileRecord(fd, nProducts) == false)
    {
        sendMessage(clientSocket, "Error unlocking file");
        return;
    }
    sendMessage(clientSocket, "Successfully added product");
    return;
}

void showAllProducts(int clientSocket)
{
    int fd = openFile(PRODUCTS_FILENAME, O_RDONLY);
    lseek(fd, 0, SEEK_SET);
    int nProducts;
    read(fd, &nProducts, sizeof(nProducts));
    struct Product product;
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
}

void updateProduct(int clientSocket)
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
            updatedProductDetails.productId = tempProduct.productId;
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
            // ! lock record
            if (lockFileRecord(fd, product.productId) == false)
            {
                close(fd);
                sendMessage(clientSocket, "Error locking file");
                return;
            }
            lseek(fd, -sizeof(updatedProductDetails), SEEK_CUR);
            write(fd, &updatedProductDetails, sizeof(updatedProductDetails));
            close(fd);
            // ! unlock record
            if (unlockFileRecord(fd, product.productId) == false)
            {
                close(fd);
                sendMessage(clientSocket, "Error unlocking file");
                return;
            }
            sendMessage(clientSocket, "Successfully updated product");
            close(fd);
            return;
        }
    }
    close(fd);
    sendMessage(clientSocket, "Product not found");
    return;
}

void deleteProduct(int clientSocket)
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
            // ! lock record
            if (lockFileRecord(fd, productId) == false)
            {
                close(fd);
                sendMessage(clientSocket, "Error locking file");
                return;
            }
            lseek(fd, -sizeof(tempProduct), SEEK_CUR);
            write(fd, &tempProduct, sizeof(tempProduct));
            close(fd);
            // ! unlock record
            if (unlockFileRecord(fd, productId) == false)
            {
                close(fd);
                sendMessage(clientSocket, "Error unlocking file");
                return;
            }
            close(fd);
            sendMessage(clientSocket, "Successfully deleted product");
            return;
        }
    }
    close(fd);
    sendMessage(clientSocket, "Product not found");
    return;
}

void addToCart(int clientSocket, struct User *user)
{
    struct CartItem cartItem;
    if (read(clientSocket, &cartItem, sizeof(cartItem)) < 0)
    {
        printf("Error in reading cart item\n");
        exit(1);
    }
    int fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    lseek(fdCarts, 0, SEEK_SET);
    int nCarts;
    read(fdCarts, &nCarts, sizeof(nCarts));
    // ! since only logged in users can add to cart, we can assume that the user exists
    // ! so does their cart such that cardId = userId
    // ! just lseek to that cart
    struct Cart cart;
    lseek(fdCarts, sizeof(nCarts) + ((user->userId - 1) * sizeof(cart)), SEEK_SET);
    read(fdCarts, &cart, sizeof(cart));
    if (cart.nProducts >= 10)
    {
        close(fdCarts);
        sendMessage(clientSocket, "Cart is full. Try removing some items first.");
    }

    int fdProducts = openFile(PRODUCTS_FILENAME, O_RDWR);
    lseek(fdProducts, 0, SEEK_SET);
    int nProducts;
    read(fdProducts, &nProducts, sizeof(nProducts));
    // ! check if product exists
    if (nProducts < cartItem.productId)
    {
        close(fdCarts);
        close(fdProducts);
        sendMessage(clientSocket, "No such product exists.");
    }
    // ! check if product already exists in cart
    bool isPresentInCart = false;
    int itemIndexInCart = -1;
    for (int i = 0; i < cart.nProducts; i++)
    {
        if (cart.productIds[i] == cartItem.productId)
        {
            isPresentInCart = true;
            itemIndexInCart = i;
            break;
        }
    }

    struct Product product;
    lseek(fdProducts, sizeof(nProducts) + ((cartItem.productId - 1) * sizeof(product)), SEEK_SET);
    read(fdProducts, &product, sizeof(product));
    // ! if product already deleted or quantity not available - then return false
    if (product.isDeleted == true || product.quantityAvailable < cartItem.quantity)
    {
        close(fdCarts);
        close(fdProducts);
        if (product.isDeleted == true)
        {
            sendMessage(clientSocket, "Product is deleted.");
        }
        else
        {
            sendMessage(clientSocket, "Product quantity not available.");
        }
    }
    // TODO - don't decrease quantity available
    // TODO - only when he checks out you should inform him that the quantity is not available
    // TODO - also check if the product is deleted or not

    // ! add product to cart
    if (isPresentInCart == true)
    {
        if (cart.quantities[itemIndexInCart] + cartItem.quantity > product.quantityAvailable)
        {
            close(fdCarts);
            close(fdProducts);
            sendMessage(clientSocket, "Product quantity not available.");
        }
        cart.quantities[itemIndexInCart] += cartItem.quantity;
        lseek(fdCarts, -sizeof(cart), SEEK_CUR);
        write(fdCarts, &cart, sizeof(cart));
        close(fdCarts);
        close(fdProducts);
        sendMessage(clientSocket, "Product added to cart.");
    }
    else
    {
        cart.productIds[cart.nProducts] = cartItem.productId;
        cart.quantities[cart.nProducts] = cartItem.quantity;
        cart.nProducts++;
        lseek(fdCarts, -sizeof(cart), SEEK_CUR);
        write(fdCarts, &cart, sizeof(cart));
        close(fdCarts);
        close(fdProducts);
        sendMessage(clientSocket, "Product added to cart.");
    }
}

void showCart(int clientSocket, struct User *user)
{
    // ! userId = cartId
    int fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    lseek(fdCarts, 0, SEEK_SET);
    int nCarts;
    read(fdCarts, &nCarts, sizeof(nCarts));
    struct Cart cart;
    int cartId = user->userId;
    lseek(fdCarts, sizeof(nCarts) + ((cartId - 1) * sizeof(cart)), SEEK_SET);
    read(fdCarts, &cart, sizeof(cart));
    close(fdCarts);

    write(clientSocket, &cart, sizeof(cart));
}

void updateCartItem(int clientSocket, struct User *user)
{
    struct CartItem cartItem;
    if (read(clientSocket, &cartItem, sizeof(cartItem)) < 0)
    {
        printf("Error in reading cart item\n");
        exit(1);
    }
    int fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    lseek(fdCarts, 0, SEEK_SET);
    int nCarts;
    read(fdCarts, &nCarts, sizeof(nCarts));

    int cartId = user->userId;
    struct Cart cart;
    lseek(fdCarts, sizeof(nCarts) + ((cartId - 1) * sizeof(cart)), SEEK_SET);
    read(fdCarts, &cart, sizeof(cart));

    // ! check if that product exists in the cart
    bool exists = false;
    int productIndex = -1;
    for (int i = 0; i < cart.nProducts; i++)
    {
        if (cart.productIds[i] == cartItem.productId)
        {
            exists = true;
            cart.quantities[i] = cartItem.quantity;
            productIndex = i;
            break;
        }
    }
    if (exists == false)
    {
        close(fdCarts);
        sendMessage(clientSocket, "Product does not exist in cart");
        return;
    }

    // ! now update / delete the product in the cart
    int newQuantity = cartItem.quantity;
    if (newQuantity == 0)
    {
        // ! delete
        for (int i = productIndex; i < cart.nProducts - 1; i++)
        {
            cart.productIds[i] = cart.productIds[i + 1];
            cart.quantities[i] = cart.quantities[i + 1];
        }
        cart.nProducts--;
        // ! update the cart
        lseek(fdCarts, -sizeof(cart), SEEK_CUR);
        write(fdCarts, &cart, sizeof(cart));
        sendMessage(clientSocket, "Cart item deleted successfully");
        close(fdCarts);
        return;
    }
    else
    {
        // ! update
        // ! check if the newQuantity is available
        int fdProducts = openFile(PRODUCTS_FILENAME, O_RDWR);
        lseek(fdProducts, 0, SEEK_SET);
        int nProducts;
        read(fdProducts, &nProducts, sizeof(nProducts));
        int productId = cartItem.productId;
        struct Product product;
        lseek(fdProducts, sizeof(nProducts) + ((productId - 1) * sizeof(product)), SEEK_SET);
        read(fdProducts, &product, sizeof(product));
        if (product.quantityAvailable < newQuantity)
        {
            close(fdCarts);
            close(fdProducts);
            sendMessage(clientSocket, "New Quantity not available");
            return;
        }
        // TODO - don't decrease quantity available or check if the product is deleted or not
        // TODO - do that only when he checks out
        cart.quantities[productIndex] = newQuantity;
        // ! update the cart
        lseek(fdCarts, -sizeof(cart), SEEK_CUR);
        write(fdCarts, &cart, sizeof(cart));
        sendMessage(clientSocket, "Cart item updated successfully");
        close(fdCarts);
        return;
    }
}

void checkout(int clientSocket, struct User *user)
{
    int cartId = user->userId;
    int fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    lseek(fdCarts, 0, SEEK_SET);
    int nCarts;
    read(fdCarts, &nCarts, sizeof(nCarts));
    struct Cart cart;
    lseek(fdCarts, sizeof(nCarts) + ((cartId - 1) * sizeof(cart)), SEEK_SET);
    read(fdCarts, &cart, sizeof(cart));
    close(fdCarts);

    int fdProducts = openFile(PRODUCTS_FILENAME, O_RDWR);
    lseek(fdProducts, 0, SEEK_SET);
    int nProducts;
    read(fdProducts, &nProducts, sizeof(nProducts));
    // ! first check if all the products are available
    for (int i = 0; i < cart.nProducts; i++)
    {
        int productId = cart.productIds[i];
        int quantity = cart.quantities[i];
        struct Product product;
        lseek(fdProducts, sizeof(nProducts) + ((productId - 1) * sizeof(product)), SEEK_SET);
        read(fdProducts, &product, sizeof(product));
        if (product.quantityAvailable < quantity || product.isDeleted == true)
        {
            close(fdProducts);
            sendMessage(clientSocket, "Products not available. Remove them from cart and try again");
            return;
        }
    }
    // ! now update the products
    for (int i = 0; i < cart.nProducts; i++)
    {
        int productId = cart.productIds[i];
        int quantity = cart.quantities[i];
        struct Product product;
        // ! lock the product
        if (lockFileRecord(fdProducts, productId - 1) == false)
        {
            close(fdProducts);
            sendMessage(clientSocket, "Error in locking product");
            return;
        }
        lseek(fdProducts, sizeof(nProducts) + ((productId - 1) * sizeof(product)), SEEK_SET);
        read(fdProducts, &product, sizeof(product));
        product.quantityAvailable -= quantity;
        lseek(fdProducts, -sizeof(product), SEEK_CUR);
        write(fdProducts, &product, sizeof(product));

        // ! unlock the product
        if (unlockFileRecord(fdProducts, productId - 1) == false)
        {
            close(fdProducts);
            sendMessage(clientSocket, "Error in unlocking product");
            return;
        }
    }
    close(fdProducts);
    // ! now update the cart
    fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    lseek(fdCarts, 0, SEEK_SET);
    read(fdCarts, &nCarts, sizeof(nCarts));
    lseek(fdCarts, sizeof(nCarts) + ((cartId - 1) * sizeof(cart)), SEEK_SET);
    read(fdCarts, &cart, sizeof(cart));
    cart.nProducts = 0;
    lseek(fdCarts, -sizeof(cart), SEEK_CUR);
    write(fdCarts, &cart, sizeof(cart));
    close(fdCarts);
    sendMessage(clientSocket, "Checkout successful");
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
                showAllProducts(clientSocket);
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