#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "database.h"

int db_start () 
{
    printf("Starting database\n");
    return 0;
}

int db_option (int option)
{
    printf("User chose option: %d\n", option);
    return 0;
}
