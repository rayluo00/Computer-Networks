#include <rpc/rpc.h>
#include <stdio.h>
#define RPC CLNT
#include "database.h"

extern CLIENT *handle;
static int *RETURN_STATUS;

int db_create (int dbType)
{
    int *arg;
    arg = &dbType;

    RETURN_STATUS = db_create_1(arg, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;
}
