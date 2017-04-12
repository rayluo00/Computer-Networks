#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "database.h"
#include <gdbm.h>
#include "proto.h"

GDBM_FILE DATABASE;

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
	printf("PUT %s | %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);
	return 0;
}

/******************************************************************************
 *
 *
 */
int db_get (struct location_params args)
{
	printf("GET %s | %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);
	return 0;
}
