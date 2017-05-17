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

int main (int argc, char **argv)
{
    char buf[1024] = "Hello World!";
    int sd;
    int buf_len = sizeof(buf);
    struct in_addr local_interface;
    struct sockaddr_in group_sock;
    
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        fprintf(stderr, "error: Unable to create socket.\n");
        exit(EXIT_FAILURE);
    }

    memset((char *)&group_sock, 0, sizeof(group_sock));
    group_sock.sin_family = AF_INET;
    group_sock.sin_addr.s_addr = inet_addr(GROUP_ADDR);
    group_sock.sin_port = htons((u_short) atoi(argv[2]));

    local_interface.s_addr = inet_addr(argv[1]);
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, 
                (char *)&local_interface, 
                sizeof(local_interface)) < 0) {
        fprintf(stderr, "error: Failed to set local interface "
                "socket options.\n");
        close(sd);
        exit(EXIT_FAILURE);
    }

    printf("Creating socket...OK.\n");

    //sd = create_server_socket(argv, local_interface, group_sock);

    if (sendto(sd, buf, buf_len, 0, (struct sockaddr *)&group_sock,
            sizeof(group_sock)) < 0) {
        fprintf(stderr, "error: Failed to send message.\n");
    } else {
        printf("SENT: %s\n", buf);
    }

    return 0;
}

