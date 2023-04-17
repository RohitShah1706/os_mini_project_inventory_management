#include "server_handler.h"

bool checkLogin(struct User *user)
{
    bool isLoggedIn = false;

    int fd = openFile(USERS_FILENAME, O_RDWR);
    struct User tempUser;
    lseek(fd, 0, SEEK_SET);
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
    return isLoggedIn;
}

bool doSignUp(struct User *user)
{
    int fd = openFile(USERS_FILENAME, O_RDWR);
    lseek(fd, 0, SEEK_SET);
    struct User tempUser;
    while (read(fd, &tempUser, sizeof(tempUser)) > 0)
    {
        if (strcmp(tempUser.email, user->email) == 0)
        {
            return false;
        }
    }
    lseek(fd, 0, SEEK_END);
    user->userId = tempUser.userId + 1;
    write(fd, user, sizeof(*user));
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