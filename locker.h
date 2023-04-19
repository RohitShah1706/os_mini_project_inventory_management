#ifndef LOCKER_H
#define LOCKER_H

#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

bool lockFileRecord(int fd, int recordNo);
bool unlockFileRecord(int fd, int recordNo);

#endif