#include <rpc/rpc.h>
#define RPC SVC
#include "database.h"

static int RETURN_STATUS;

int db_create (int);

int *db_create_svc (int *dbType, struct svc_req *rqstp)
{
    RETURN_STATUS = db_create(*dbType);
    return &RETURN_STATUS;
}
