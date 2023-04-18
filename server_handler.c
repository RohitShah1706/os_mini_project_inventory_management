#include "server_handler.h"

bool checkLogin(struct User *user)
{
    bool isLoggedIn = false;

    int fd = openFile(USERS_FILENAME, O_RDWR);

    // ! first read nUsers
    lseek(fd, 0, SEEK_SET);
    int nUsers = 0;
    read(fd, &nUsers, sizeof(nUsers));
    struct User tempUser;
    while (read(fd, &tempUser, sizeof(tempUser)) > 0)
    {
        if (strcmp(tempUser.email, user->email) == 0 && strcmp(tempUser.password, user->password) == 0)
        {
            isLoggedIn = true;
            user->userId = tempUser.userId;
            user->isAdmin = tempUser.isAdmin;
            strcpy(user->address, tempUser.address);
            user->age = tempUser.age;
            strcpy(user->phoneNo, tempUser.phoneNo);
            break;
        }
    }
    close(fd);
    return isLoggedIn;
}

bool doSignUp(struct User *user)
{
    int fdUsers = openFile(USERS_FILENAME, O_RDWR);
    int fdCarts = openFile(CARTS_FILENAME, O_RDWR);
    // ! check nUsers and increment it
    lseek(fdUsers, 0, SEEK_SET);
    lseek(fdCarts, 0, SEEK_END);
    int nUsers, nCarts;
    read(fdUsers, &nUsers, sizeof(nUsers));
    read(fdCarts, &nCarts, sizeof(nCarts));
    nUsers++;
    nCarts++;

    // ! search for existing user
    struct User tempUser;
    while (read(fdUsers, &tempUser, sizeof(tempUser)) > 0)
    {
        if (strcmp(tempUser.email, user->email) == 0)
        {
            return false;
        }
    }
    // ! write nUsers
    lseek(fdUsers, 0, SEEK_SET);
    write(fdUsers, &nUsers, sizeof(nUsers));

    // ! write nCarts
    lseek(fdCarts, 0, SEEK_SET);
    write(fdCarts, &nCarts, sizeof(nCarts));

    // ! write user
    lseek(fdUsers, 0, SEEK_END);
    user->userId = nUsers;
    write(fdUsers, user, sizeof(*user));
    close(fdUsers);

    // ! write cart
    lseek(fdCarts, 0, SEEK_END);
    struct Cart cart;
    cart.userId = nUsers;
    cart.nProducts = 0;
    write(fdCarts, &cart, sizeof(cart));
    return true;
}

bool handleSignUp(int clientSocket)
{
    struct User user;
    int status = read(clientSocket, &user, sizeof(user));
    if (status < 0)
    {
        printf("Error in reading user\n");
        return false;
    }
    user.isAdmin = false;
    printf("User received: %s\n", user.email);
    return doSignUp(&user);
}

bool handleLogin(int clientSocket, struct User *user)
{
    int status = read(clientSocket, user, sizeof(*user));
    if (status < 0)
    {
        printf("Error in reading user\n");
        return false;
    }
    printf("User received: %s\n", user->email);
    return checkLogin(user);
}

void *handleClientConnection(void *arg)
{
    int clientSocket = *(int *)arg;
    int choice;
    char *message;
    struct User user;
    bool isLoggedIn = false;

    while (1)
    {
        if (read(clientSocket, &choice, sizeof(choice)) < 0)
        {
            printf("Error in reading choice\n");
            exit(1);
        }
        switch (choice)
        {
        case 1:
            if (handleSignUp(clientSocket))
            {
                message = "User created successfully";
            }
            else
            {
                message = "Error in creating user";
            }
            sendMessage(clientSocket, message);
            break;
        case 2:
            isLoggedIn = handleLogin(clientSocket, &user);
            if (write(clientSocket, &isLoggedIn, sizeof(isLoggedIn)) < 0)
            {
                printf("Error in writing login status\n");
                exit(1);
            }
            if (isLoggedIn)
            {
                // ! send user details
                if (write(clientSocket, &user, sizeof(user)) < 0)
                {
                    printf("Error in writing user details\n");
                    exit(1);
                }
                // ! switch to handleUserMenu
                handleUserMenu(clientSocket, &user);
            }
            break;
        case 3:
            break;
        default:
            break;
        }
        if (choice == 3)
        {
            break;
        }
    }
}