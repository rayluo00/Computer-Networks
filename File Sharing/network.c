#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "network.h"

int create_leech_socket (char *hostname, int port)
{
	int sock;
	int flag = 1;
	struct sockaddr_in sock_info;
	struct hostent *host_table_ptr;
	struct protoent *protocol_table_ptr;

	memset((char *)&sock_info, 0, sizeof(sock_info));
	sock_info.sin_family = AF_INET;
	sock_info.sin_port = htons((u_short)port);

	host_table_ptr = gethostbyname(hostname);
	if ((char*) host_table_ptr == NULL) {
		fprintf(stderr, "error: Unable to find host in table.\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&sock_info.sin_addr, host_table_ptr->h_addr, host_table_ptr->h_length);
	
	if (((long int)(protocol_table_ptr = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "error: Unable to map tcp protocol number.\n");
		exit(EXIT_FAILURE);
	}

	sock = socket(PF_INET, SOCK_STREAM, protocol_table_ptr->p_proto);
	if (sock < 0) {
		fprintf(stderr, "error: Unable to create socket.\n");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
		&flag, sizeof(int)) < 0) {
		fprintf(stderr, "error: Unable to set socket option.\n");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr *)&sock_info, sizeof(sock_info)) < 0) {
		fprintf(stderr, "error: Unable to connect onto socket.\n");
		exit(EXIT_FAILURE);
	}

	return sock;
}

int create_seed_socket (int port)
{
	int flag = 1;
	int sock;
	struct sockaddr_in sock_info;
	struct hostent *host_table_ptr;
	struct protoent *protocol_table_ptr;

	memset((char *)&sock_info, 0, sizeof(sock_info));
	sock_info.sin_family = AF_INET;
	sock_info.sin_addr.s_addr = INADDR_ANY;
	sock_info.sin_port = htons((u_short)port);

	if (((long int)(protocol_table_ptr = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "");
		exit(EXIT_FAILURE);
	}

	// Create a new socket.
	sock = socket(PF_INET, SOCK_STREAM, protocol_table_ptr->p_proto);
	if (sock < 0) {
		fprintf(stderr, "error: Unable to create socket.\n");
		exit(EXIT_FAILURE);
	}

	// Set socket options
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
		&flag, sizeof(int)) < 0) {
		fprintf(stderr, "error: Unable to set socket options.\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sock, (struct sockaddr *)&sock_info, sizeof(sock_info)) < 0) {
		fprintf(stderr, "error: Server bind failed.\n");
		exit(EXIT_FAILURE);
	}

	if (listen(sock, 6) < 0) {
		fprintf(stderr, "error: Unable to listen onto socket.\n");
		exit(EXIT_FAILURE);
	}

	return sock;
}

FILE *open_split_file (int split_count)
{
	char filename[256];	
	snprintf(filename, sizeof(filename), 
			 "./tmp/split_%d.txt", split_count);

	FILE *new_split_file = fopen(filename, "w+");

	if (new_split_file == NULL) {
		fprintf(stderr, "error: Unable to open split file.\n");
		exit(EXIT_FAILURE);
	}

	return new_split_file;
}

void split_file (int argc, char **argv) 
{
	int leechers = 3;
	int file_size = 0;
	int split_size;
	int split_count = 0;
	char ch;
	FILE *txt_file = fopen(argv[1], "r");
	FILE *segment;
	struct stat txt_stats;

	if (txt_file == NULL) {
		fprintf(stderr, "error: Unable to open file %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &txt_stats) < 0) {
		fprintf(stderr, "error: Unable to get file stats.\n");
		exit(EXIT_FAILURE);
	}

	printf("FILE SIZE: %d\n", txt_stats.st_size);
	//TODO change (leechers+1) to accomodate for real leechers
	split_size = ceil(txt_stats.st_size / leechers);
	segment = open_split_file(split_count++);

	while ((ch = fgetc(txt_file)) != EOF) {
		fputc(ch, segment);
		file_size++;

		if (file_size > split_size) {
			file_size = 0;
			fclose(segment);
			segment = open_split_file(split_count++);
		}
	}
}
