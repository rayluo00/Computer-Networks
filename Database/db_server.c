#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "database.h"
#include <gdbm.h>
#include "proto.h"

GDBM_FILE DATABASE;

int db_start () 
{
    printf("Starting database\n");
	
	DATABASE = gdbm_open("467DB", 0, GDBM_NEWDB, 0644, 0);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to open GDBM");
		exit(EXIT_FAILURE);
	}

    return 0;
}

int db_create (struct db_args args)
{
	printf("DBTYPE: %d | DBNAME: %s\n", args.DB_TYPE, args.DB_NAME);
	return 0;
}
