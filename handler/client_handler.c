#include "client_handler.h"

void signUp(int sockfd)
{
    // ! send signup request to server
    struct User user;
    printf("Enter email with no spaces: ");
    scanf("%s", user.email);
    printf("Enter password with no spaces: ");
    scanf("%s", user.password);
    printf("Enter age: ");
    scanf("%d", &user.age);
    printf("Enter phone number: ");
    scanf("%s", user.phoneNo);
    printf("Enter address: ");
    scanf("%s", user.address);

    // ! send user to server
    int status = write(sockfd, &user, sizeof(user));
    if (status < 0)
    {
        printf("Error in sending user\n");
        exit(1);
    }
    receiveMessage(sockfd);
}

bool login(int sockfd)
{
    bool isLoggedIn = false;
    struct User user;
    printf("Enter email with no spaces: ");
    scanf("%s", user.email);
    printf("Enter password with no spaces: ");
    scanf("%s", user.password);

    // ! send user to server
    int status = write(sockfd, &user, sizeof(user));
    if (status < 0)
    {
        printf("Error in sending user\n");
        exit(1);
    }
    if (read(sockfd, &isLoggedIn, sizeof(isLoggedIn)) < 0)
    {
        printf("Error in reading login status\n");
        exit(1);
    }
    return isLoggedIn;
}

void handleServerConnection(int sockfd)
{
    // ! first handle login / signup
    int choice;
    bool isLoggedIn = false;
    bool isAdmin = false;
    struct User user;
    while (1)
    {
        if (isLoggedIn)
        {
            showUserMenu(sockfd, &user, &isLoggedIn, &isAdmin);
        }
        printf("0. Exit\n");
        printf("1. Sign up\n");
        printf("2. Login\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (write(sockfd, &choice, sizeof(choice)) < 0)
        {
            printf("Error in sending choice\n");
            exit(1);
        }
        switch (choice)
        {
        case 1:
            signUp(sockfd);
            break;
        case 2:
            if (!login(sockfd))
            {
                printf("Wrong password or email. Try again.\n");
            }
            else
            {
                // TODO - check for isAdmin
                if (read(sockfd, &user, sizeof(user)) < 0)
                {
                    printf("Error in reading user details\n");
                    exit(1);
                }
                isLoggedIn = true;
                isAdmin = user.isAdmin;
                printf("Login successful\n");
            }
            break;
        case 0:
            break;
        default:
            break;
        }
        if (choice == 0)
        {
            break;
        }
    }
}