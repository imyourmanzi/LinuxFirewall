/*
 * File: proj2driver/fw421_reset.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * C program wrapper for fw421_reset syscall.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrappers.h"

int main() {
    long retval = fw421_reset_syscall();
    if (retval) {
        fprintf(stderr, "%ld:%s\n", retval, strerror(retval));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
