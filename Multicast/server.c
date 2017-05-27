#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

#define GROUP_ADDR "239.46.7.13"

const char

int main (int argc, char **argv)
{
    char buf[1024];
    int igmp_sd, i, tcp_sd, tcp_sd2, sd, sd2, size;
    int buf_len = sizeof(buf);
    struct in_addr local_interface;
    struct sockaddr_in group_sock, tcp_sock;
    fd_set read_set, active_set;

    if (argc < 2) {
        fprintf(stderr, "error: Missing parameters.\n");
        exit(EXIT_FAILURE);
    }

    // IGMP
    igmp_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (igmp_sd < 0) {
        fprintf(stderr, "error: socket() failed\n");
        exit(EXIT_FAILURE);
    }

    memset((char *)&group_sock, 0, sizeof(group_sock));
    group_sock.sin_family = AF_INET;
    group_sock.sin_addr.s_addr = inet_addr(GROUP_ADDR);
    group_sock.sin_port = htons((u_short) atoi(argv[2]));

    local_interface.s_addr = inet_addr(argv[1]);
    if (setsockopt(igmp_sd, IPPROTO_IP, IP_MULTICAST_IF, 
                (char *)&local_interface, 
                sizeof(local_interface)) < 0) {
        fprintf(stderr, "error: setsockopt() failed.\n");
        close(igmp_sd);
        exit(EXIT_FAILURE);
    }

    printf("Create IGMP socket...DONE.\n");

    snprintf(buf, buf_len, "SERVER_SENDTO_MSG");
    sendto(igmp_sd, buf, buf_len, 0, (struct sockaddr *)&group_sock,
            sizeof(group_sock));

    // TPC/IP
    tcp_sd = create_server(atoi(argv[3]));
    tcp_sd2 = create_server(atoi(argv[4]));

    FD_ZERO(&active_set);
    FD_SET(0, &active_set);
    FD_SET(igmp_sd, &active_set);
    FD_SET(tcp_sd, &active_set);
    FD_SET(tcp_sd2, &active_set);

    while (1) {
        size = sizeof(tcp_sock);
        sd = accept(tcp_sd, (struct sockaddr *)&tcp_sock, &size);
        sd2 = accept(tcp_sd2, (struct sockaddr *)&tcp_sock, &size);

        if (sd < 0) {
            fprintf(stderr, "error: Failed to accept socket.\n");
            exit(EXIT_FAILURE);
        }

        FD_SET(sd, &active_set);
        FD_SET(sd2, &active_set);
        printf("Accept sockets...DONE.\n");

        while (1) {
            read_set = active_set;
            if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
                fprintf(stderr, "error: Client disconnected.\n");
            }

            memset(buf, 0, 1024);
            for (i = 0; i < FD_SETSIZE; i++) {
                if (FD_ISSET(i, &read_set)) {
                    if (i == 0) {
                        if (read(0, buf, 1024) > 0) {
                            sendto(igmp_sd, buf, buf_len, 0, 
                                      (struct sockaddr *)&group_sock,
                                      sizeof(group_sock));
                            send(sd, buf, buf_len, 0);
                            printf("SEND: %s\n", buf);
                        }
                    }
                    else if (i == sd) {
                        if (read(i, buf, 1024) > 0) {
                            printf("CLIENT 1: %s\n", buf);
                        }
                    }
                    else if (i == sd2) {
                        if (read(i, buf, 1024) > 0) {
                            printf("CLIENT 2: %s\n", buf);
                        } 
                    }
                }
            }
        }
    }

    close(igmp_sd);
    close(tcp_sd);
    close(tcp_sd2);
    close(sd);
    close(sd2);

    return 0;
}

