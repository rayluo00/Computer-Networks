#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "network.h"

#define PORT 46713

int main (int argc, char **argv)
{
	int i, size, new_sock, new_sock2, sock, sock2, status;
	char buffer[1024];
	fd_set read_fd_set, active_fd_set;
	struct sockaddr_in sock_info;

	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	system("exec rm -r ./tmp/*");	
	sock = create_seed_socket(atoi(argv[1]));
	sock2 = create_seed_socket(atoi(argv[2]));

	FD_ZERO(&active_fd_set);
	FD_SET(0, &active_fd_set);
	FD_SET(sock, &active_fd_set);
	FD_SET(sock2, &active_fd_set);

	while (1) {
		size = sizeof(sock_info);
		new_sock = accept(sock, (struct sockaddr *)&sock_info, &size);
		new_sock2 = accept(sock2, (struct sockaddr *)&sock_info, &size);

		if (new_sock < 0 || new_sock2 < 0) {
			fprintf(stderr, "error: Failed to accept socket.\n");
			exit(EXIT_FAILURE);
		}

		FD_SET(new_sock, &active_fd_set);
		FD_SET(new_sock2, &active_fd_set);
		printf("ACCPET Socket %d\n", new_sock);
		printf("ACCPET Socket %d\n", new_sock2);

		while (1) {
			read_fd_set = active_fd_set;
			memset(buffer, 0, 1024);

			if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
				fprintf(stderr, "error: Client disconnected.\n");
				//exit(EXIT_FAILURE);
			}

			for (i = 0; i < FD_SETSIZE; i++) {
				if (FD_ISSET(i, &read_fd_set)) {
					// Keyboard input
					if (i == 0) {
						if (read(0, buffer, 1024) > 0) {
							if (!strncmp(buffer, "split", 5)) {
								split_file(argc, argv);
								printf("Splitting done.\n");
								merge_files(3);
								printf("Merging done.\n");
							} else {
								send(new_sock, buffer, strlen(buffer), 0);
								send(new_sock2, buffer, strlen(buffer), 0);
							}
						}
					}
					// Data from an established connection
					else {
						if (read(i, buffer, 1024) > 0) {
							printf("SEED %d: %s", i, buffer);
						}
					}
				}
			}
		}
	}

	// NOTE: Methods for splitting and merging a txt file.
	//split_file(argc, argv);
	//merge_files(3);

	close(sock);
	close(sock2);

	return 0;
}
