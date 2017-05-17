#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCAL_ADDR "127.0.0.1"
#define GROUP_ADDR "239.46.7.13"

int main (int argc, char **argv) {
    char buf[1024];
    int sd;
    int buf_len = sizeof(buf);
    struct ip_mreq group;
    struct sockaddr_in local_sock;

    int reuse = 1;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        fprintf(stderr, "error: Failed to create socket.\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                   sizeof(reuse)) < 0) {
        fprintf(stderr, "error: Unable to set SO_REUSEADDR.\n");
        close(sd);
        exit(EXIT_FAILURE);
    }

    memset((char *)&local_sock, 0, sizeof(local_sock));
    local_sock.sin_family = AF_INET;
    local_sock.sin_port = htons((u_short) atoi(argv[2]));
    local_sock.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&local_sock, 
             sizeof(local_sock))) {
        fprintf(stderr, "error: Failed to bind socket.\n");
        close(sd);
        exit(EXIT_FAILURE);
    }

    group.imr_multiaddr.s_addr = inet_addr(GROUP_ADDR);
    group.imr_interface.s_addr = inet_addr(argv[1]);
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                   (char *)&group, sizeof(group)) < 0) {
        fprintf(stderr, "error: Unable to add multicast group.\n");
        close(sd);
        exit(EXIT_FAILURE);
    }


    //sd = create_client_socket(argv, group, local_sock);

    if (read(sd, buf, buf_len) < 0) {
        fprintf(stderr, "error: Failed to read from server.\n");
        close(sd);
        exit(EXIT_FAILURE);
    }

    printf("READ: %s\n", buf);

    return 0;
}
