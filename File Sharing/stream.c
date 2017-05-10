#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "network.h"

/*
	A -> B : 11111 -> 22222
	B -> C : 22222 -> 33333
	C -> A : 33333 -> 11111
 */

int main (int argc, char **argv) {
	int i, size, seed, leech, status;
	char buffer[1024];
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
		read_fd_set = active_fd_set;
		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "error: select() failed.\n");
		}

		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
				memset(buffer, 0, 1024);

				// Keyboard input
				if (i == 0) {
					if (fgets(buffer, 1024, stdin) != NULL) {
						status = write(leech, buffer, strlen(buffer)+1);
						status = write(seed, buffer, strlen(buffer)+1);

						if (status < 0) {
							fprintf(stderr, "error: Failed server write.\n");
						}
					}
				}
				// Accept connection to the seed
				else if (i == seed) {
					printf("ACCEPT SOCKET %d\n", i);

					int new_sock;

					size = sizeof(sock_info);
					new_sock = accept(i, (struct sockaddr *)&sock_info, &size);

					if (new_sock < 0) {
						fprintf(stderr, "error: Failed new socket accept.\n");
						exit(EXIT_FAILURE);
					}

					FD_SET(new_sock, &active_fd_set);
				}
				// Data from established connection
				else {
					if (read(i, buffer, sizeof(buffer)) > 0) {
						printf("STREAM %d: %s", i, buffer);
					} else {
						close(i);
						FD_CLR(i, &active_fd_set);
					}
				}
			}
		}
	}

	close(seed);
	close(leech);

	return 0;
}
