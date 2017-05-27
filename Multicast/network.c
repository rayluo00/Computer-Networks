#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "network.h"

int create_client (char *hostname, int port)
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

int create_server (int port)
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
		fprintf(stderr, "error: Unable to get proto name.\n");
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

