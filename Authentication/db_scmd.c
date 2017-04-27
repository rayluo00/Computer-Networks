/********************************************************************
 * db_scmd.c
 *
 * Author: Raymond Weiming Luo
 * CSCI 467 - Computer Networks II
 *
 * RPC status handling for the commands that the client can call
 * across the network from the server. The authentication step
 * for the client accessing the server is handled in this file
 * to mediate the UID and machine name of the client process.
 *
 *******************************************************************/

#include <rpc/rpc.h>
#define RPC SVC
#include "database.h"
#include <sys/types.h>

static int RETURN_STATUS;
static int RET_FAIL = -1;

int db_start (void);
int db_create (struct db_args);
int db_open (struct db_args);
int db_close(void);
int db_put(struct location_params);
int db_get(struct location_params);
int db_auth(void);

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

int *db_close_1_svc (void *arg, struct svc_req *rqstp)
{
    RETURN_STATUS = db_close();
    return &RETURN_STATUS;
}

int *db_put_1_svc (location_params *args, struct svc_req *rqstp)
{
    RETURN_STATUS = db_put(*args);
    return &RETURN_STATUS;
}

int *db_get_1_svc (location_params *args, struct svc_req *rqstp)
{
    RETURN_STATUS = db_get(*args);
    return &RETURN_STATUS;
}

int *db_auth_1_svc (void *args, struct svc_req *rqstp)
{
	struct authunix_parms *sys_cred;
	uid_t uid;
	char *host;
	int host_ret = -1;

	sys_cred = (struct authunix_parms *) rqstp->rq_clntcred;
	uid = sys_cred->aup_uid;
	host = sys_cred->aup_machname;

	printf("AUTH UID: %d : %s\n", uid, host);

	if (uid < 1001 || uid > 1010) {
		printf("error: Can not authenticate uid %d\n", uid);
		return &RET_FAIL;
	}

	if (host == NULL) {
		fprintf(stderr, "error: No hostname given\n");
		return &RET_FAIL;
	}

	/*
	if (strncmp(host, "osbdata", 7) == 0 || strncmp(host, "networking", 10) == 0) {
		host_ret = 0;
	}
	*/

	if (strncmp(host, "osbdata", 7) == 0) {
		host_ret = 0;
	}	
	
	if (host_ret == -1) {
		fprintf(stderr, "error: Unable to authenticate hostname %s\n", host);	
		return &RET_FAIL;
	}

	RETURN_STATUS = db_auth();
	return &RETURN_STATUS;
}

