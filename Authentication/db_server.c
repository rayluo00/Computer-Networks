#include <sys/types.h>
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "database.h"
#include <gdbm.h>
#include <rpcsvc/rusers.h>
#include <rpc/auth.h>

extern CLIENT *handle;
GDBM_FILE DATABASE;

/******************************************************************************
 *
 *
 */
int db_auth ()
{
	printf("AUTHENTICATE client handle\n");

	if (handle == NULL) {
		fprintf(stderr, "error: No client handle\n");
		return -1;
	}

	//printf("AUTH: %s\n", handle->cl_auth->ah_cred.oa_base);
	printf("RET %d\n", handle->cl_auth->ah_cred.oa_base);

	/*
	if (handle->cl_auth->ah_ops->ah_validate) {
		printf("AUTHENTICATE success\n");
		return 0;
	}
	*/

	//printf("AUTHENTICATE fail\n");
	return 0;
}

/******************************************************************************
 *
 *
 */
int db_start () 
{
    printf("START database\n");
    return 0;
}

/******************************************************************************
 *
 *
 */
int db_create (struct db_args args)
{
	printf("CREATE %d - %s\n", args.DB_TYPE, args.DB_NAME);
	
	DATABASE = gdbm_open(args.DB_NAME, 0, GDBM_WRCREAT, 0644, 0);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to create GDBM");
		return -1;
	}

	gdbm_close(DATABASE);
	
	return 0;
}

/******************************************************************************
 *
 *
 */
int db_open (struct db_args args)
{
	printf("OPEN %d - %s\n", args.DB_TYPE, args.DB_NAME);

	DATABASE = gdbm_open(args.DB_NAME, 0, GDBM_WRITER, 0644, 0);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to open GDBM");
		return -1;
	}

	return 0;
}

/******************************************************************************
 *
 *
 */
int db_close () 
{
	if (DATABASE == NULL) {
		fprintf(stderr, "error: No database open, nothing to close\n");
		return -1;
	}

	printf("CLOSE Database\n");
	gdbm_close(DATABASE);
	DATABASE = NULL;
	return 0;
}

/******************************************************************************
 *
 *
 */
int db_put (struct location_params args)
{
	printf("PUT %s| %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);

	if (DATABASE == NULL) {
		return -1;
	}
	
	datum key;
	datum data;
	int print_ret;
	char data_buf[1024];

	key.dptr = args.NAME;
	key.dsize = strlen(args.NAME) + 1;
	
	print_ret = snprintf(data_buf, 1024, "%s,%s,%s", args.CITY, args.STATE, args.TYPE);

	if (print_ret < 0) {
		fprintf(stderr, "error: snprintf failed to create data buffer\n");
		return -1;
	}

	printf("PUT DATA BUF %s\n", data_buf);

	data.dptr = data_buf;
	data.dsize = 1025;
	
	if (gdbm_store(DATABASE, key, data, GDBM_INSERT)) {
		fprintf(stderr, "error: Unable to store key %s\n", key);
		return -1;
	}

	return 0;
}

/******************************************************************************
 *
 *
 */
int db_get (struct location_params args)
{
	printf("GET %s | %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);
	
	if (DATABASE == NULL) {
		return -1;
	}

	datum key;
	datum data;

	key.dptr = args.NAME;
	key.dsize = strlen(args.NAME) + 1;

	data = gdbm_fetch(DATABASE, key);

	if (data.dptr == NULL) {
		fprintf(stderr, "error: Unable to fetch key %s\n", key);
		return -1;
	}

	printf("GET KEY: %s | DATA: %s\n", key.dptr, data.dptr);

	return 0;
}
