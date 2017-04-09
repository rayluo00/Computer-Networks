#include <rpc/rpc.h>
#define RPC SVC
#include "database.h"

static int RETURN_STATUS;

int db_start (void);
int db_option (int);

int *db_start_1_svc (void *arg, struct svc_req *rqstp)
{
    RETURN_STATUS = db_start();
    return &RETURN_STATUS;
}

int *db_option_1_svc (int *option, struct svc_req *rqstp)
{
    RETURN_STATUS = db_option(*option);
    return &RETURN_STATUS;
}
