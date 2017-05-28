#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

#define GROUP_ADDR "239.46.7.13"

int main (int argc, char **argv) {
    char buf[1024];
    int igmp_sd, i, sd, sd2;
    int reuse = 1;
    int buf_len = sizeof(buf);
    struct ip_mreq group;
    struct sockaddr_in local_sock;
    fd_set read_set, active_set;

    if (argc < 2) {
        fprintf(stderr, "error: Missing parameters.\n");
        exit(EXIT_FAILURE);
    }

    // IGMP
    igmp_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (igmp_sd < 0) {
        fprintf(stderr, "error: socket() failed.\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(igmp_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                   sizeof(reuse)) < 0) {
        fprintf(stderr, "error: setsockopt() failed.\n");
        close(igmp_sd);
        exit(EXIT_FAILURE);
    }

    memset((char *)&local_sock, 0, sizeof(local_sock));
    local_sock.sin_family = AF_INET;
    local_sock.sin_port = htons((u_short) atoi(argv[2]));
    local_sock.sin_addr.s_addr = INADDR_ANY;
    if (bind(igmp_sd, (struct sockaddr *)&local_sock, 
             sizeof(local_sock)) < 0) {
        fprintf(stderr, "error: Failed to bind socket.\n");
        close(igmp_sd);
        exit(EXIT_FAILURE);
    }

    group.imr_multiaddr.s_addr = inet_addr(GROUP_ADDR);
    group.imr_interface.s_addr = inet_addr(argv[1]);
    if (setsockopt(igmp_sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                   (char *)&group, sizeof(group)) < 0) {
        fprintf(stderr, "error: setsockopt() groups failed.\n");
        close(igmp_sd);
        exit(EXIT_FAILURE);
    }

    printf("Creating IGMP socket...DONE.\n");
    
    // TCP/IP
    sd = create_client(argv[1], atoi(argv[3]));

    FD_ZERO(&active_set);
    FD_SET(0, &active_set);
    FD_SET(sd, &active_set);
    FD_SET(igmp_sd, &active_set);

    while (1) {
        read_set = active_set;

        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            fprintf(stderr, "error: Failed to select.\n");
            close(sd);
            close(igmp_sd);
            exit(EXIT_FAILURE);
        }

        memset(buf, 0, 1024);
        for (i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == 0) {
                    if (read(0, buf, buf_len) > 0) {
                        send(sd, buf, strlen(buf), 0);
                    }
                }
                else if (i == igmp_sd) {
                    if (read(i, buf, buf_len) > 0) {
                        printf("%s\n", buf);
                    }
                }
            }
        }
    }

    close(igmp_sd);
    close(sd);

    return 0;
}

