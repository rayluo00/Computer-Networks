/********************************************************************
 * network.c
 *
 * Author: Raymond Weiming Luo
 *
 * File holding the utility functionalities of the file sharing 
 * program. These functions are implemented from all the peers
 * using specific functions.
 *
 */

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

/********************************************************************
 * Create a client TCP socket connection. Return the socket descriptor
 * to the original caller.
 */
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

	// Get hostname into the host table
	host_table_ptr = gethostbyname(hostname);
	if ((char*) host_table_ptr == NULL) {
		fprintf(stderr, "error: Unable to find host in table.\n");
		exit(EXIT_FAILURE);
	}

	// Use TCP protocol
	memcpy(&sock_info.sin_addr, host_table_ptr->h_addr, host_table_ptr->h_length);	
	if (((long int)(protocol_table_ptr = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "error: Unable to map tcp protocol number.\n");
		exit(EXIT_FAILURE);
	}

	// Create a socket
	sock = socket(PF_INET, SOCK_STREAM, protocol_table_ptr->p_proto);
	if (sock < 0) {
		fprintf(stderr, "error: Unable to create socket.\n");
		exit(EXIT_FAILURE);
	}

	// Set socket options
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
		&flag, sizeof(int)) < 0) {
		fprintf(stderr, "error: Unable to set socket option.\n");
		exit(EXIT_FAILURE);
	}

	// Connect to a listening socket
	if (connect(sock, (struct sockaddr *)&sock_info, sizeof(sock_info)) < 0) {
		fprintf(stderr, "error: Unable to connect onto socket.\n");
		exit(EXIT_FAILURE);
	}

	return sock;
}

/********************************************************************
 * Create a TCP socket connection with the peers for file sharing.
 * Returns the socket descriptor once a connection has been 
 * esatblished.
 */
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
		fprintf(stderr, "error: Unable to set TCP protocol.\n");
		exit(EXIT_FAILURE);
	}

	// Create a new socket
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

	// Bind the the socket descriptor
	if (bind(sock, (struct sockaddr *)&sock_info, sizeof(sock_info)) < 0) {
		fprintf(stderr, "error: Server bind failed.\n");
		exit(EXIT_FAILURE);
	}

	// Listen for any client socket connections
	if (listen(sock, 6) < 0) {
		fprintf(stderr, "error: Unable to listen onto socket.\n");
		exit(EXIT_FAILURE);
	}

	return sock;
}

/********************************************************************
 * Open one of the file peices based on the id of the file peice.
 */
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

/********************************************************************
 * Given a file, split it to N number of peices. Where N is the 
 * number of peers connecting to the seed. Currently, N is statically
 * set to 2 for only 2 peers.
 */
void split_file (int argc, char **argv) 
{
	int leechers = 2;
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

	// Calculate the size of each peice
	split_size = ceil(txt_stats.st_size / leechers);
	segment = open_split_file(split_count++);

	printf("FILE: %d | SEGMENT: %d\n", (int) txt_stats.st_size, split_size);

	// Split the files according to the calculated size of each peice
	while ((ch = fgetc(txt_file)) != EOF) {
		fputc(ch, segment);
		file_size++;

		if (file_size > split_size) {
			file_size = 0;
			fclose(segment);
			segment = open_split_file(split_count++);
		}
	}

	fclose(segment);
	fclose(txt_file);
}

/********************************************************************
 * Merge the files given a path name to the file peices. Merges all
 * the text files into a new file within the given path called
 * 'merge.txt'.
 */
void merge_files (int user_count, char *file_path)
{
	int split_count = 0;
	char ch;
	char filename[256];
	FILE *txt_file;
	FILE *merge_file;

	// Open and truncate or create a new file called merge.txt
	snprintf(filename, sizeof(filename), "%s/merge.txt", file_path);
	merge_file = fopen(filename, "w+");
	printf("Merging to %s\n", filename);

	if (merge_file == NULL) {
		fprintf(stderr, "error: Unable to open merge file.\n");
		exit(EXIT_FAILURE);
	}

	// Merge all the file peices from the directory into the merge file
	for (; split_count < user_count; split_count++) {
		memset(filename, 0, 256);
		snprintf(filename, sizeof(filename), 
				 "%s/f%d.txt", file_path, split_count);
		printf("Reading %s\n", filename);

		txt_file = fopen(filename, "r");

		if (txt_file == NULL) {
			fprintf(stderr, "error: Unable to open split file.\n");
			exit(EXIT_FAILURE);
		}

		// Write the data from the peices into the merge file
		while ((ch = fgetc(txt_file)) != EOF) {
			fputc(ch, merge_file);
		}

		fclose(txt_file);
	}

	fclose(merge_file);
}
