#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define MASK 15

CLIENT *handle;

typedef struct ret {
    int svr_mode;
    int status;
    int error;
} ret_val;


/* Client procedures */
int encode(int clientID, int option);
ret_val decode(int code);
int get_option(int clientID);
int db_create(int dbType, char *dbName);
int db_open(int dbType, char *dbName);
int db_close();
// TODO: Fill in params
int db_put();
int db_get();

/* Server procedures */
int db_start();
int db_option(int option);

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

int get_option (int clientID)
{
    char buffer[1024];
    int option;
    int code;

    do {
        printf("$ ");
        fgets(buffer, sizeof(buffer), stdin);

        if (tolower(*buffer) == 'q') {
            exit(0);
        }

        option = atoi(buffer);
    } while (option < 0 || option > 3);

    code = encode(clientID, option);
    return db_option(code);
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

    display_options();

    while (online) {
        while (status > -1) {
            ret = decode(get_option(clientID));
            status = get_status(ret);
        }
    }

    printf("DB offline\n");
    
    return 0;
}
