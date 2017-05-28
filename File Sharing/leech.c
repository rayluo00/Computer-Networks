/********************************************************************
 * leech.c
 *
 * Author: Raymond Weiming Luo
 *
 * Leech-side peer of file sharing P2P model, retrieve half of the
 * text document from the seed while simultaneously sending it to
 * the another peer (stream). Then retrieve the missing half of text
 * from the stream. Computes the time performance using clock(3).
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include "network.h"

#define LEECH_FILE = "./file1/f1.txt";
#define PORT 46713

/********************************************************************
 * Main function to set socket connections to P2P and retrieve input
 * data from both the seed and stream side. Send the text data to the
 * stream when recieved and get the missing peice of text from the 
 * stream. Merge both pieces together to get the full text document.
 */
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
	clock_t start, end;
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
						// Quit program
						if (!strncmp(buffer, "q", 1)) {
							printf("Quitting program.\n");
							close(sock);
							close(sock2);
							exit(EXIT_SUCCESS);
						}
						// Display processing time
						else if (!(strncmp(buffer, "time", 4))) {
							printf("Time elapsed: %f\n", timer);
						}
						// Merge peices to one file
						else if (!(strncmp(buffer, "merge", 5))) {
							merge_files(2, "./file1");
						}
						// Reset processing timer
						else if (!strncmp(buffer, "reset", 5)) {
							timer = 0;
						} 
						// Send input to peers
						else {
							send(sock, buffer, strlen(buffer), 0);
							send(sock2, buffer, strlen(buffer), 0);
						}
					}
				}
				// Stream
				else if (i == sock) {
					if ((status = read(i, buffer, 1024)) > 0) {
						out_file = fopen("./file1/f1.txt", "a");
						start = clock();
						fputs(buffer, out_file);
						end = clock();
						timer += ((double) (end - start)) / CLOCKS_PER_SEC;
						fclose(out_file);
					}
				}
				// Seed
				else if (i == sock2) {
					if ((status = read(i, buffer, 1024)) > 0) {
						if (!strncmp(buffer, "NOSPLIT_FLAG", 12)) {
							flag = 1;
							printf("No split flag = ON\n");
						} else {
							out_file = fopen("./file1/f0.txt", "a");
							start = clock();
							fputs(buffer, out_file);
							end = clock();
							timer += ((double) (end - start)) / CLOCKS_PER_SEC;
							fclose(out_file);
							
							// Only send data to stream if flag is not given
							if (!flag) {
								send(sock, buffer, strlen(buffer), 0);	
							}
						}
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
