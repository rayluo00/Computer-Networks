#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include "network.h"

#define LEECH_FILE = "./file1/f1.txt";
#define PORT 46713

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	int i, sock, sock2, status;
	int flag = 0;
	char buffer[1024];
	char *hostname = "localhost";
	double timer = 0;
	struct sockaddr_in sock_info;
	FILE *out_file;
	fd_set active_fd_set, read_fd_set;

	sock = create_leech_socket(hostname, atoi(argv[1]));
	sock2 = create_leech_socket(hostname, atoi(argv[2]));
	
	FD_ZERO(&active_fd_set);
	FD_SET(0, &active_fd_set);
	FD_SET(sock, &active_fd_set);
	FD_SET(sock2, &active_fd_set);

	while (1) {
		read_fd_set = active_fd_set;

		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "error: Unable to select from file descriptors.\n");
			exit(EXIT_FAILURE);
		}

		memset(buffer, 0, 1024);
		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
				// Keyboard input
				if (i == 0) {
					if (read(0, buffer, 1024) > 0) {
						if (!strncmp(buffer, "q", 1)) {
							printf("Quitting program.\n");
							close(sock);
							close(sock2);
							exit(EXIT_SUCCESS);
						}
						else if (!(strncmp(buffer, "time", 4))) {
							printf("Time elapsed: %f\n", timer);
						}
						else if (!(strncmp(buffer, "merge", 5))) {
							merge_files(2, "./file1");
						} 
						else {
							send(sock, buffer, strlen(buffer), 0);
							send(sock2, buffer, strlen(buffer), 0);
						}
					}
				}
				// STREAM
				else if (i == sock) {
					if ((status = read(i, buffer, 1024)) > 0) {
						out_file = fopen("./file1/f1.txt", "a");
						start_time();
						fputs(buffer, out_file);
						timer += end_time();
						fclose(out_file);
					}
				}
				// SEED
				else if (i == sock2) {
					if ((status = read(i, buffer, 1024)) > 0) {
						out_file = fopen("./file1/f0.txt", "a");
						start_time();
						fputs(buffer, out_file);
						timer += end_time();
						fclose(out_file);
						
						send(sock, buffer, strlen(buffer), 0);	
					}
				}
				// Socket input
				else {
					if ((status = read(i, buffer, 1024)) > 0) {
						printf("CLIENT %d: %s\n", i, buffer);
					}
				}
			}
		}
	}

	close(sock);
	close(sock2);

	return 0;
}
