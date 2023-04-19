#include "locker.h"

key_t getKey()
{
    key_t key = ftok(".", 'a');
    if (key < 0)
    {
        perror("ftok");
        exit(1);
    }
    return key;
}

int getSemaphoreId()
{
    int semid = semget(getKey(), PRODUCTS_TOTAL_ALLOWED, 0666 | IPC_CREAT);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }
    return semid;
}

struct sembuf getLock(int recordNo)
{
    return (struct sembuf){
        .sem_num = recordNo,
        .sem_op = -1, // ! 0 = wait for the semaphore to be available
        .sem_flg = 0,
    };
}

struct sembuf getUnLock(int recordNo)
{
    return (struct sembuf){
        .sem_num = recordNo,
        .sem_op = 1,
        .sem_flg = 0,
    };
}

bool lockFileRecord(int fd, int recordNo)
{
    bool isLocked = false;
    int semid = getSemaphoreId();
    struct sembuf lockSem = getLock(recordNo);
    int status = semop(semid, &lockSem, 1);
    if (status < 0)
    {
        perror("semop");
        return isLocked;
    }
    isLocked = true;
    return isLocked;
}
bool unlockFileRecord(int fd, int recordNo)
{
    bool isUnlocked = false;
    int semid = getSemaphoreId();
    struct sembuf unlockSem = getUnLock(recordNo);
    int status = semop(semid, &unlockSem, 1);
    if (status < 0)
    {
        perror("semop");
        return isUnlocked;
    }
    isUnlocked = true;
    return isUnlocked;
}