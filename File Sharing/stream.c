#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "network.h"

#define STREAM_FILE = "./file3/f3.txt";

/*
	A -> B : 11111 -> 22222
	B -> C : 22222 -> 33333
	C -> A : 33333 -> 11111
 */

int main (int argc, char **argv) {
	int i, size, seed, leech, status, new_sock;
	char buffer[1024];
	double timer = 0;
	FILE *out_file;
	struct sockaddr_in sock_info;
	fd_set read_fd_set, active_fd_set;

	system("exec rm -f ./tmp/*");

	seed = create_seed_socket(atoi(argv[1]));
	leech = create_leech_socket("localhost", atoi(argv[2]));

	FD_ZERO(&active_fd_set);
	FD_SET(0, &active_fd_set);
	FD_SET(seed, &active_fd_set);
	FD_SET(leech, &active_fd_set);

	while (1) {
		size = sizeof(sock_info);
		new_sock = accept(seed, (struct sockaddr *)&sock_info, &size);
		if (new_sock < 0) {
			fprintf(stderr, "error: Failed to accept socket.\n");
			exit(EXIT_FAILURE);
		}
		printf("ACCEPT Socket %d\n", new_sock);
		FD_SET(new_sock, &active_fd_set);

		while (1) {
			read_fd_set = active_fd_set;
			memset(buffer, 0, 1024);

			if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
				fprintf(stderr, "error: select() failed.\n");
			}

			for (i = 0; i < FD_SETSIZE; i++) {
				if (FD_ISSET(i, &read_fd_set)) {
					// Keyboard input
					if (i == 0) {
						if (read(0, buffer, 1024) > 0) {
							if (!strncmp(buffer, "q", 1)) {
								printf("Quitting program.\n");
								close(seed);
								close(leech);
								close(new_sock);
								exit(EXIT_SUCCESS);
							}
							else if (!strncmp(buffer, "time", 4)) {
								printf("Time elapsed: %f\n", timer);
							} else {
								send(leech, buffer, strlen(buffer), 0);
								send(new_sock, buffer, strlen(buffer), 0);
							}
						}
					}
					else if (i == leech) {
						if ((status = read(i, buffer, 1024)) > 0) {
							//printf("SEED\n");
							out_file = fopen("./file2/stream.txt", "a");
							start_time();
							fputs(buffer, out_file);
							send(new_sock, buffer, strlen(buffer), 0);
							timer += end_time();
							fclose(out_file);
						}
					}
					else if (i == new_sock) {
						if ((status = read(i, buffer, 1024)) > 0) {
							//printf("LEECH\n");
							out_file = fopen("./file2/stream.txt", "a");
							start_time();
							fputs(buffer, out_file);
							timer += end_time();
							fclose(out_file);
						}
					}
					// Data from established connection
					else {
						if ((status = read(i, buffer, 1024)) > 0) {
							printf("STREAM %d: %s", i, buffer);
						}
					}
				}
			}
		}
	}

	close(seed);
	close(leech);

	return 0;
}
