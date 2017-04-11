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

int db_create (struct db_args args)
{
    db_args *arg;
    arg = &args;

    RETURN_STATUS = db_create_1(arg, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;
}

int db_open (struct db_args args)
{
    db_args *arg;
    arg = &args;

    RETURN_STATUS = db_open_1(arg, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;
}

int db_close () 
{
	RETURN_STATUS = db_close_1(0, handle);
    return RETURN_STATUS == 0 ? 0 : *RETURN_STATUS;

}
