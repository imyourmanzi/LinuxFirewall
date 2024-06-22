/*
 * File: proj2driver/fw421_block_port.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * C program wrapper for fw421_block_port syscall.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/uapi/linux/in.h"
#include "wrappers.h"

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <protocol> <direction> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int proto = -1;
    int dir = -1;
    unsigned short port = 0;

    /* map correct protocol */
    if (strncmp(argv[1], "tcp", 3) == 0) {
        proto = IPPROTO_TCP;
    } else if (strncmp(argv[1], "udp", 3) == 0) {
        proto = IPPROTO_UDP;
    }

    /* map correct direction */
    if (strncmp(argv[2], "out", 3) == 0) {
        dir = 0;
    } else if (strncmp(argv[2], "in", 2) == 0) {
        dir = 1;
    }

    /* convert port number */
    port = (unsigned short) strtoul(argv[3], NULL, 10);

    long retval = fw421_block_port_syscall(proto, dir, port);
    if (retval) {
        fprintf(stderr, "%ld:%s\n", retval, strerror(retval));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
