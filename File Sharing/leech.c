#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include "network.h"

#define PORT 46713

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	int i, sock, sock2, server_ret;
	fd_set active_fd_set, read_fd_set;
	struct sockaddr_in sock_info;
	char *hostname = "localhost";
	char buf[1024];

	sock = create_leech_socket(hostname, atoi(argv[1]));
	sock2 = create_leech_socket(hostname, atoi(argv[2]));
	
	FD_ZERO(&active_fd_set);
	FD_SET(0, &active_fd_set);
	FD_SET(sock, &active_fd_set);
	FD_SET(sock2, &active_fd_set);

	while (1) {
		read_fd_set = active_fd_set;
		memset(buf, 0, 1024);

		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "error: Unable to select from file descriptors.\n");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
				// Keyboard input
				if (i == 0) {
					if (read(0, buf, 1024) > 0) {
						server_ret = send(sock, buf, strlen(buf), 0);
						server_ret = send(sock2, buf, strlen(buf), 0);

						if (server_ret < 0) {
							fprintf(stderr, "error: Failed write to server.\n");
						}
					}
				}
				// Socket input
				else {
					if (read(i, buf, 1024) > 0) {
						printf("CLIENT %d: %s\n", i, buf);
					}
				}
			}
		}
	}

	close(sock);
	close(sock2);

	return 0;
}
