/********************************************************************
 * db_server.c
 *
 * Author: Raymond Weiming Luo
 * CSCI 467: Computer Networks II
 *
 * Server-side application for the ONC RPC database program. The
 * sever will authenticate the client before allowing a connection
 * to the GDBM database. Once the client is allows a connect to the
 * server, the server will recieve commands from the client by
 * passing a struct holding the information for utilizing a database.
 *
 *******************************************************************/

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
#include <rpc/auth_unix.h>

GDBM_FILE DATABASE;

int db_auth ()
{
	printf("AUTHENTICATE client\n");
	return 0;
}

int db_start () 
{
    printf("START database\n");
    return 0;
}

int db_create (struct db_args args)
{
	printf("CREATE %d:%s\n", args.DB_TYPE, args.DB_NAME);
	
	DATABASE = gdbm_open(args.DB_NAME, 0, GDBM_WRCREAT, 0644, 0);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to create GDBM\n");
		return -1;
	}

	gdbm_close(DATABASE);
	
	return 0;
}

int db_open (struct db_args args)
{
	printf("OPEN %d:%s\n", args.DB_TYPE, args.DB_NAME);

	DATABASE = gdbm_open(args.DB_NAME, 0, GDBM_WRITER, 0644, 0);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: Failed to open GDBM\n");
		return -1;
	}

	return 0;
}

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

int db_put (struct location_params args)
{
	printf("PUT %s| %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);

	if (DATABASE == NULL) {
		fprintf(stderr, "error: No opened database\n");
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

int db_get (struct location_params args)
{
	printf("GET %s | %s | %s | %s\n", args.NAME, args.CITY, args.STATE, args.TYPE);
	
	if (DATABASE == NULL) {
		fprintf(stderr, "error: No database open\n");
		return -1;
	}

	datum key;
	datum data;
	datum nextKey;

	if (!strncmp(args.NAME, "*", 1)) {
		key = gdbm_firstkey(DATABASE);

		while (key.dptr) {
			data = gdbm_fetch(DATABASE, key);

			if (data.dptr == NULL) {
				fprintf(stderr, "error: Unable to fecth key %s\n", key);
				return -1;
			}

			printf("GET KEY: %s | DATA: %s\n", key.dptr, data.dptr);

			nextKey = gdbm_nextkey(DATABASE, key);
			key = nextKey;
		}
	} else {
		key.dptr = args.NAME;
		key.dsize = strlen(args.NAME) + 1;

		data = gdbm_fetch(DATABASE, key);

		if (data.dptr == NULL) {
			fprintf(stderr, "error: Unable to fetch key %s\n", key);
			return -1;
		}

		printf("GET KEY: %s | DATA: %s\n", key.dptr, data.dptr);
	}

	return 0;
}
