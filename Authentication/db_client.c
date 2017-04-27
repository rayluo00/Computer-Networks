/********************************************************************
 * db_client.c
 *
 * Author: Raymond Weiming Luo
 * CSCI 467: Computer Networks II
 *
 * Client-side application for an ONC RPC program to recieve user
 * commands to interact with the GDBM database. The program has
 * a client handle used to track the different clients.
 *
 * Client commands:
 *    mk [DATABASE NAME]: 
 *        Create a new GDBM database. Given the database name.
 *
 *    open [DATABSE NAME]: 
 *        Open the GDBM database. Given the database name.
 *
 *    put NAME,CITY,STATE,TYPE: 
 *        Add the NAME as a key into the GDBM database with the 
 *        values seperated by commas from the database that is 
 *        opened.
 *
 *    get NAME,CITY,STATE,TYPE: 
 *        Get the data that are stored as values from the key 
 *        NAME in the GDBM database that is opened.
 *
 *******************************************************************/

#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <gdbm.h>
#include <string.h>
#include <rpcsvc/rusers.h>
#include <rpc/auth.h>
#include <rpc/auth_unix.h>
#include "database.h"
#define MASK 15

CLIENT *handle;

struct db_args DB_INFO;
struct location_params LOCATION_DATA;

typedef struct ret {
    int svr_mode;
    int status;
    int error;
} ret_val;

/* Client procedures */
int encode(int clientID, int option);
ret_val decode(int code);
int get_cmd(int clientID);
int get_status(ret_val ret);
char **parse_args(char *line);
struct db_args fill_db_struct(char **cmd);
struct location_params fill_location_params(char **cmd);

/* Server procedures */
int db_start();
int db_create(struct db_args args);
int db_open(struct db_args args);
int db_close();
int db_put(struct location_params params);
int db_get(struct location_params params);
int db_auth();

int encode (int clientID, int option)
{
    return (clientID << 8) + option;
}

ret_val decode (int code)
{
    ret_val ret;

    ret.svr_mode = code & MASK;
    code = code >> 4;
    ret.error = code & MASK;
    code = code >> 4;
    ret.status = code & MASK;

    return ret;
}

char **parse_args (char *line)
{
	int x = 0;
	int idx;
	int argc = 0;
	int inWord = 0;
	int argLens = strlen(line);
	char *token;
	char **cmd;	

	for (idx = 0; idx < argLens; idx++) {
		if (line[idx] == ' ' || line[idx] == 0) {
			if (inWord) {
				line[idx] = 0;
				inWord = 0;
			}
		} else {
			if (!inWord) {
				inWord = 1;
				argc++;
			}
		}
	}

	cmd = (char **) malloc(sizeof(char *) * (++argc));

	if (cmd == NULL) {
		fprintf(stderr, "error: malloc failed during argument parsing");
		db_close();
		exit(EXIT_FAILURE);
	}

	inWord = 0;
	
	for (idx = 0; idx < argLens; idx++) {
		if (!inWord && (line[idx] != ' ' || line[idx] != 0)) {
			token = &line[idx];
			cmd[x] = token;
			x++;
			inWord = 1; 
		} 
		else if (inWord && (line[idx] == ' ' || line[idx] == 0)) {
			inWord = 0;	
		}
	}

	cmd[x] = 0;

	return cmd;
}

struct db_args fill_db_struct (char **cmd)
{
	struct db_args args;
	int dbType;
	char *dbName;

	dbType = atoi(cmd[1]);

	if (dbType == 0 && cmd[1] != "0") {
		dbType = 1;
		dbName = cmd[1];
	} 
	else if (cmd[2] != NULL) {
		dbName = cmd[2];
	}

	args.DB_TYPE = dbType;
	args.DB_NAME = dbName;

	return args;
}

void cleanup (char *name, char *city, char *state, char *type)
{
	free(name);
	free(city);
	free(state);
	free(type);
}

struct location_params fill_location_params (char **cmd)
{
	struct location_params params;
	int idx = 0;
	int token_len;
	char *name;
	char *city;
	char *state;
	char *type;
	char *token;
	char *cmd_params = strdup(cmd[1]);

	while ((token = strsep(&cmd_params, ",")) != NULL) {
		printf("TOK[%d] : %s\n", idx, token);
		
		if (token != '\0') {
			token_len = strlen(token) + 1;

			switch (idx) {
				case 0:
					name = (char *) malloc(sizeof(char *) * token_len);

					if (name == NULL) {
						params.VALID = -1;
						fprintf(stderr, "error: Unable to malloc\n");
						return params;
					}

					strcpy(name, token);
					break;
				case 1:
					city = (char *) malloc(sizeof(char *) * token_len);
					
					if (city == NULL) {
						params.VALID = -1;
						fprintf(stderr, "error: Unable to malloc\n");
						return params;
					}

					strcpy(city, token);
					break;
				case 2:
					state = (char *) malloc(sizeof(char *) * token_len); 
					
					if (state == NULL) {
						params.VALID = -1;
						fprintf(stderr, "error: Unable to malloc\n");
						return params;
					}

					strcpy(state, token);
					break;
				case 3:
					type = (char *) malloc(sizeof(char *) * token_len);
					
					if (type == NULL) {
						params.VALID = -1;
						fprintf(stderr, "error: Unable to malloc\n");
						return params;
					}

					strcpy(type, token);
					break;
				default:
					fprintf(stderr, "error: Invalid location params %s\n", token);
			}
		}

		idx++;
	}

	if (idx != 4) {
		params.VALID = -1;
		fprintf(stderr, "error: Missing location param arguments\n");
		return params;
	}

	params.NAME = name;
	params.CITY = city;
	params.STATE = state;
	params.TYPE = type;
	params.VALID = 0;	

	// TODO: Free location params after stored into gdbm
	//free(cmd_params);
	
	return params;
}

int get_cmd (int clientID)
{
    char buffer[1024];
	char *input;
	char **cmd;
	//int code;
	struct db_args args;
	struct location_params params;

    do {
        printf("$ ");
        fgets(buffer, sizeof(buffer), stdin);

        if (tolower(*buffer) == 'q') {
			db_close();
            exit(0);
        }
    } while (buffer == NULL);

    //code = encode(clientID, cmd);
	//return db_option(code);

	cmd = parse_args(buffer);

	/*	
	int x = 0;	
	while (cmd[x] != NULL) {
		printf("CMD[%d] : %s\n", x, cmd[x]);
		x++;
	}
	*/

	if (!strncmp(cmd[0], "mk", 2)) {
		args = fill_db_struct(cmd);
		db_create(args);
	}
	else if (!strncmp(cmd[0], "open", 4)) {
		args = fill_db_struct(cmd);
		db_open(args);
	}
	else if (!strncmp(cmd[0], "close", 5)) {
		db_close();
	}
	else if (!strncmp(cmd[0], "put", 3)) {
		params = fill_location_params(cmd);

		if (params.VALID == 0) {
			db_put(params);
		}
	}
	else if (!strncmp(cmd[0], "get", 3)) {
		params = fill_location_params(cmd);
		
		if (params.VALID == 0) {
			db_get(params);
		}
	} else {
		fprintf(stderr, "error: Invalid command\n");
	}

	free(cmd);
	return 0;	
}

int get_status (ret_val ret)
{
    int retVal = 0;

    if (ret.svr_mode != 0) {
        retVal = ret.svr_mode;
    }

    if (ret.status != 0) {
        return -1;
    }
    else if (ret.error != 0) {
        if (ret.error == 1) {
            printf("error: Invalid option\n");
            return 0;
        }
        else if (ret.error == 1) {
            printf("error: Unknown client id\n");
            return -2;
        }
    }

    return retVal;
}

int main (int argc, char **argv) 
{
    int progNum = 24670113;
    int verNum = 1;
    int online = 1;
    int clientID;
    int status;
	int groupSize;
	int intsend, intrecv;
    char *localhost = "localhost";
    char *host = 0;
    ret_val ret;
	gid_t gids[100];
	unsigned int svr_auth;
	enum clnt_stat clnt_auth;
	struct timeval timeout;

    if (host == 0) {
		if (argv[1] != NULL) {
			printf("CONNECT : %s\n", argv[1]);
        	host = argv[1];
		} else {
			printf("CONNECT : %s\n", localhost);
			host = localhost;
		}
    }

	if ((groupSize = getgroups(NGROUPS_MAX, gids)) < 0) {
		fprintf(stderr, "error: Unable to get gids\n");
		exit(EXIT_FAILURE);
	} else {
		groupSize = (groupSize > 16) ? 16 : groupSize;
	}

    handle = clnt_create(host, progNum, verNum, "tcp");

    if (handle == NULL) {
        fprintf(stderr, "error: Unable to connect to host: %s\n", host);
        exit(EXIT_FAILURE);
    }

	//handle->cl_auth = authunix_create(host, getuid(), getgid(), groupSize, gids);
	handle->cl_auth = authunix_create_default();

	if (db_auth() < 0) {
		fprintf(stderr, "error: Invalid authentication credentials\n");
		exit(EXIT_FAILURE);
	}
    
	if ((clientID = db_start()) < 0) {
        fprintf(stderr, "error: Unable to start database");
        exit(EXIT_FAILURE);
    }

    while (online) {
        while (status > -1) {
            get_cmd(clientID);
			//ret = decode(get_cmd(clientID));
            //status = get_status(ret);
        }
    }

	db_close();
    printf("DB offline\n");
 
	auth_destroy(handle->cl_auth);
	clnt_destroy(handle);
   
    return 0;
}
