#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 46713

FILE *open_split_file(int split_count);
void split_file(int argc, char **argv);
void merge_files(int user_count);
int create_socket(int port);

int main (int argc, char **argv)
{
	int i, size;
	int sock, sock2;
	char buffer[1024];
	fd_set read_fd_set, active_fd_set;
	struct sockaddr_in sock_info;

	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	system("exec rm -r ./tmp/*");	
	sock = create_socket(PORT);
	sock2 = create_socket((PORT+1));

	FD_ZERO(&active_fd_set);
	FD_SET(sock, &active_fd_set);
	FD_SET(sock2, &active_fd_set);

	while (1) {
		read_fd_set = active_fd_set;

		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
			fprintf(stderr, "error: Client disconnected.\n");
			//exit(EXIT_FAILURE);
		}

		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_fd_set)) {
				// Accepting a new connection
				if (i == sock || i == sock2) {
					printf("ACCPETING SOCKET %d\n", i);
					int new_sock;

					size = sizeof(sock_info);
					new_sock = accept(i, (struct sockaddr *) &sock_info, &size);

					if (new_sock < 0) {
						fprintf(stderr, "error: Unable to accept new socket.\n");
						exit(EXIT_FAILURE);
					}

					FD_SET(new_sock, &active_fd_set);
				}
				// Data from an established connection
				else {
					if (read(i, buffer, sizeof(buffer)) > 0) {
						printf("SERVER: %s\n", buffer);
					} else {
						close(i);
					}
				}
			}
		}
	}

	// NOTE: Methods for splitting and merging a txt file.
	//split_file(argc, argv);
	//merge_files(3);

	return 0;
}

int create_socket (int port)
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
		fprintf(stderr, "");
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

	if (listen(sock, 12) < 0) {
		fprintf(stderr, "error: Unable to listen onto socket.\n");
		exit(EXIT_FAILURE);
	}

	return sock;
}

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

void split_file (int argc, char **argv) 
{
	char ch;
	FILE *txt_file = fopen(argv[1], "r");
	FILE *split_file;
	struct stat txt_stats;
	int leechers = 3;
	int file_size = 0;
	int split_size;
	int split_count = 0;

	if (txt_file == NULL) {
		fprintf(stderr, "error: Unable to open file %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &txt_stats) < 0) {
		fprintf(stderr, "error: Unable to get file stats.\n");
		exit(EXIT_FAILURE);
	}

	printf("FILE SIZE: %d\n", txt_stats.st_size);
	//TODO change (leechers+1) to accomodate for real leechers
	split_size = ceil(txt_stats.st_size / leechers);
	split_file = open_split_file(split_count++);

	while ((ch = fgetc(txt_file)) != EOF) {
		fputc(ch, split_file);
		file_size++;

		if (file_size > split_size) {
			file_size = 0;
			fclose(split_file);
			split_file = open_split_file(split_count++);
		}
	}

	fclose(split_file);
	fclose(txt_file);
}

void merge_files (int user_count)
{
	int split_count = 0;
	char ch;
	char filename[256];
	FILE *merge_file = fopen("./tmp/merge.txt", "w+");
	FILE *txt_file;

	if (merge_file == NULL) {
		fprintf(stderr, "error: Unable to open merge file.\n");
		exit(EXIT_FAILURE);
	}

	for (; split_count < user_count; split_count++) {
		snprintf(filename, sizeof(filename), 
				 "./tmp/split_%d.txt", split_count);

		txt_file = fopen(filename, "r");

		if (txt_file == NULL) {
			fprintf(stderr, "error: Unable to open split file.\n");
			exit(EXIT_FAILURE);
		}

		while ((ch = fgetc(txt_file)) != EOF) {
			fputc(ch, merge_file);
		}

		fclose(txt_file);
	}

	fclose(merge_file);
}
