#include "server_handler.h"

int openFile(char *fileName, int flags)
{
    int fd = open(fileName, flags);
    if (fd < 0)
    {
        printf("Error in opening file\n");
        exit(1);
    }
    return fd;
}

bool checkLogin(struct User *user)
{
    // TODO - check for correct login
    bool isLoggedIn = false;

    int fd = openFile(USERS_FILENAME, O_RDWR);
    struct User tempUser;
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &tempUser, sizeof(tempUser)) > 0)
    {
        if (strcmp(tempUser.email, user->email) == 0 && strcmp(tempUser.password, user->password) == 0)
        {
            isLoggedIn = true;
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
    user.isAdmin = false;
    if (status < 0)
    {
        printf("Error in reading user\n");
        return false;
    }
    printf("User received: %s\n", user.email);
    // TODO - save received user
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