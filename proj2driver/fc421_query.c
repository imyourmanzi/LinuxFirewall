/*
 * File: proj2driver/fc421_query.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-25
 * Assignment: Project 2
 * Description:
 * C program wrapper for fc421_query syscall.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrappers.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    long retval = fc421_query_syscall(argv[1]);
    if (retval < 0) {
        fprintf(stderr, "%ld:%s\n", -retval, strerror(-retval));
        return EXIT_FAILURE;
    } else {
        fprintf(stdout, "%ld attempts\n", retval);
        return EXIT_SUCCESS;
    }

}
