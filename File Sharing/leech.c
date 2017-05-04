#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 46713

int create_socket(char *hostname, int port);

int main (int argc, char **argv)
{
	int sock;
	fd_set active_fd_set, read_fd_set;
	struct sockaddr_in sock_info;

	sock = create_socket(PORT);

	if (connect(sock, (struct sockaddr *)&sockinfo, sizeof(sock_info)) < 0) {
		fprintf(stderr, "error: Unable to connect onto socket descriptor.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int create_socket (char *hostname, int port)
{
	int sock;
	int flag = 1;
	struct sockaddr_in sock_info;
	struct hostent *host_table_ptr;
	struct protoent *protocol_table_ptr;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	host_table_ptr = gethostbyname(hostname);
	
	if ((char*) host_table_ptr == NULL) {
		fprintf(stderr, "error: Unable to find host in table.\n");
		exit(EXIT_FAILURE);
	}

	// TODO memset here
	// TODO getprotobyname

	if (sock < 0) {
		fprintf(stderr, "error: Unable to create socket.\n");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(sock. SOL_SOCKET, SO_REUSEADDR, 
		&flag, sizeof(int)) < 0) {
		fprintf(stderr, "error: Unable to set socket option.\n");
		exit(EXIT_FAILURE);
	}
}
