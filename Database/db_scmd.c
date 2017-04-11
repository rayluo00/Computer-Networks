#include <rpc/rpc.h>
#define RPC SVC
#include "database.h"

static int RETURN_STATUS;

int db_start (void);
int db_create (struct db_args);
int db_open (struct db_args);

int *db_start_1_svc (void *arg, struct svc_req *rqstp)
{
    RETURN_STATUS = db_start();
    return &RETURN_STATUS;
}

int *db_create_1_svc (db_args *args, struct svc_req *rqstp)
{
    RETURN_STATUS = db_create(*args);
    return &RETURN_STATUS;
}

int *db_open_1_svc (db_args *args, struct svc_req *rqstp)
{
    RETURN_STATUS = db_open(*args);
    return &RETURN_STATUS;
}
