#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <gdbm.h>
#include "proto.h"
#include "database.h"
#define MASK 15

CLIENT *handle;
struct db_args DB_INFO;

typedef struct ret {
    int svr_mode;
    int status;
    int error;
} ret_val;

/* Client procedures */
void display_options();
int encode(int clientID, int option);
ret_val decode(int code);
int get_cmd(int clientID);
int get_status(ret_val ret);

/* Server procedures */
int db_start();
int db_create(struct db_args args);

void display_options ()
{
    printf("===================================\n"
            "DATABASE OPTIONS\n"
            "1. Create Database.\n"
            "2. Open Database.\n"
            "3. Close Database.\n"
            "===================================\n\n");
}

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

	if (argc > 0) {
		argc++;
	}

	cmd = (char **) malloc(sizeof(char *) * argc);

	if (cmd == NULL) {
		fprintf(stderr, "error: malloc failed during argument parsing");
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

int get_cmd (int clientID)
{
    char buffer[1024];
	char *input;
	char **cmd;
	//int code;

    do {
        printf("$ ");
        fgets(buffer, sizeof(buffer), stdin);

        if (tolower(*buffer) == 'q') {
            exit(0);
        }
    } while (buffer == NULL);

    //code = encode(clientID, cmd);
	//return db_option(code);

	cmd = parse_args(buffer);

	int x = 0;	
	while (cmd[x] != NULL) {
		printf("CMD[%d] : %s\n", x, cmd[x]);
		x++;
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
    char *localhost = "localhost";
    char *host = 0;
    ret_val ret;

    if (host == 0) {
        host = localhost;
    }

    handle = clnt_create(host, progNum, verNum, "tcp");

    if (!handle) {
        fprintf(stderr, "error: Unable to connect to host: %s\n", host);
        exit(EXIT_FAILURE);
    }

    if ((clientID = db_start()) < 0) {
        fprintf(stderr, "error: Unable to start database");
        exit(EXIT_FAILURE);
    }

    //display_options();

    while (online) {
        while (status > -1) {
            ret = decode(get_cmd(clientID));
            status = get_status(ret);
        }
    }

    printf("DB offline\n");
    
    return 0;
}
