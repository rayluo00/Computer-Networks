#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "database.h"
#include <gdbm.h>

int db_start () 
{
    printf("Starting database\n");
    return 0;
}

int db_option (int option)
{
	switch (option) {
		case 0:
			printf("Forbidden area...\n");
			break;
		case 1:
			printf("Create database...\n");
			break;
		case 2:
			printf("Open database...\n");
			break;
		case 3:
			printf("Close database...\n");
			break;
		default:
			printf("Invalid option.");
	}	
    
    return 0;
}
