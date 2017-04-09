#include <rpc/rpc.h>
#include <stdio.h>
#define RPC CLNT
#include "database.h"

extern CLIENT *handle;
static int *RETURN_STATUS;

int db_start ()
{
    RETURN_STATUS = db_start_1(0, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;
}

int db_option (int option)
{
    int *arg;
    arg = &option;

    RETURN_STATUS = db_option_1(arg, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;
}
