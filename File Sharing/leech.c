#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 46713

int create_socket(char *hostname, int port);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	int i;
	int server_ret;
	int sock;
	int port = PORT + atoi(argv[1]);
	fd_set active_fd_set, read_fd_set;
	struct sockaddr_in sock_info;
	char *hostname = "localhost";
	char buf[1024];

	sock = create_socket(hostname, port);
	
	FD_ZERO(&active_fd_set);
	FD_SET(0, &active_fd_set);
	FD_SET(sock, &active_fd_set);

	while (1) {
		read_fd_set = active_fd_set;
		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "error: Unable to select from file descriptors.\n");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
				memset(buf, 0, 1024);
				printf("");

				// Keyboard input
				if (i == 0) {
					if (fgets(buf, 1024, stdin) != NULL) {
						server_ret = write(sock, buf, strlen(buf) + 1);

						if (server_ret < 0) {
							fprintf(stderr, "error: Failed write to server.\n");
							exit(EXIT_FAILURE);
						}
					}
				}
				// Socket input
				else {
					if (read(sock, buf, 1024) > 0) {
						printf("CLIENT: %s\n", buf);
					}
					else {
						close(sock);
						FD_CLR(sock, &active_fd_set);
					}
				}
			}
		}
	}

	close(sock);

	return 0;
}

int create_socket (char *hostname, int port)
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
