/*
 * File: proj2driver/connect.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-05-07
 * Assignment: Project 2
 * Description:
 * C program wrapper for connect syscall.
 *
 * Adapted from code on Beej's Guide to Network Programming
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <port> <protocol> <version>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int e;
    struct sockaddr sa;
    int sock_type;
    int sock;

    /* set currently known info */
    memset(&sa, 0, sizeof sa);

    if (*argv[3] == '4') {
        sa.sa_family = AF_INET;
        ((struct sockaddr_in *)&sa)->sin_port = htons((unsigned short)strtol(argv[1], NULL, 10));
    } else if (*argv[3] == '6') {
        sa.sa_family = AF_INET6;
        ((struct sockaddr_in6 *)&sa)->sin6_port = htons((uint16_t)strtol(argv[1], NULL, 10));
    } else {
        fprintf(stderr, "Version must be '4' or '6'\n");
        return EXIT_FAILURE;
    }

    if (strncmp(argv[2], "tcp", 3) == 0) {
        sock_type = SOCK_STREAM;
    } else if (strncmp(argv[2], "udp", 3) == 0) {
        sock_type = SOCK_DGRAM;
    } else {
        fprintf(stderr, "Protocol must be 'tcp' or 'udp'\n");
        return EXIT_FAILURE;
    }

    /* get the socket file descriptor */
    sock = socket(sa.sa_family, sock_type, 0);
    e = errno;
    if (e != 0) {
        fprintf(stderr, "socket:%d:%s\n", e, strerror(e));
        return EXIT_FAILURE;
    }

    /* connect to the address */
    connect(sock, &sa, sizeof sa);
    e = errno;
    if (e != 0) {
        fprintf(stderr, "connect:%d:%s\n", e, strerror(e));
        return EXIT_FAILURE;
    }

    return 0;
}
