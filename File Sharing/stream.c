/********************************************************************
 * stream.c
 *
 * Author: Raymond Weiming Luo
 *
 * Retrieve half of the text data from the seed and destribute it to
 * the connected leech peer. Then get the missing half of the text data
 * from the leech and merge the two prices to obtain a full text
 * document. Performance is computed using clock(3).
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <unistd.h>
#include "network.h"

#define STREAM_FILE = "./file3/f3.txt";

int main (int argc, char **argv) {
	int i, size, seed, leech, status, new_sock;
	int flag = 0;
	char buffer[1024];
	double timer = 0;
	FILE *out_file;
	clock_t start, end;
	struct sockaddr_in sock_info;
	fd_set read_fd_set, active_fd_set;

	// Remove peices from temp directory
	system("exec rm -f ./tmp/*");

	// Establish TCP connection to peers
	seed = create_seed_socket(atoi(argv[1]));
	leech = create_leech_socket("localhost", atoi(argv[2]));

	// Set active file descriptors
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

			if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
				fprintf(stderr, "error: select() failed.\n");
			}

			memset(buffer, 0, 1024);
			for (i = 0; i < FD_SETSIZE; i++) {
				if (FD_ISSET(i, &read_fd_set)) {
					// Keyboard input
					if (i == 0) {
						if (read(0, buffer, 1024) > 0) {
							// Quit program
							if (!strncmp(buffer, "q", 1)) {
								printf("Quitting program.\n");
								close(seed);
								close(leech);
								close(new_sock);
								exit(EXIT_SUCCESS);
							}
							// Display processing time
							else if (!strncmp(buffer, "time", 4)) {
								printf("Time elapsed: %f\n", timer);
							}
							// Merge files
							else if (!strncmp(buffer, "merge", 5)) {
								merge_files(2, "./file2");
							}
							// Reset the processing timer
							else if (!strncmp(buffer, "reset", 5)) {
								timer = 0;
							} 
							// Send input to leech and seed
							else {
								send(leech, buffer, strlen(buffer), 0);
								send(new_sock, buffer, strlen(buffer), 0);
							}
						}
					}
					// Seed
					else if (i == leech) {
						if ((status = read(i, buffer, 1024)) > 0) {
							// Retrieve flag message from seed to not send data to leech peer
							if (!strncmp(buffer, "NOSPLIT_FLAG", 12)) {
								flag = 1;
								printf("No split flag = ON\n");
							} else {
								out_file = fopen("./file2/f1.txt", "a");
								start = clock();
								fputs(buffer, out_file);
								end = clock();
								timer += ((double) (end - start)) / CLOCKS_PER_SEC;
								fclose(out_file);

								if (!flag) {
									send(new_sock, buffer, strlen(buffer), 0);	
								}
							}
						}
					}
					// Leech
					else if (i == new_sock) {
						if ((status = read(i, buffer, 1024)) > 0) {
							out_file = fopen("./file2/f0.txt", "a");
							start = clock();
							fputs(buffer, out_file);
							end = clock();
							timer += ((double) (end - start)) / CLOCKS_PER_SEC;
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
	close(new_sock);

	return 0;
}
