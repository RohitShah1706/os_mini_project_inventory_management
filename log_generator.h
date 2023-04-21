#ifndef LOG_GENERATOR_H
#define LOG_GENERATOR_H

#include "data.h"
#include "tableFormatters/fort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void adminLog();
void clientLog(int userId, ft_table_t *table);

#endif
