/*
 * File: proj2driver/fc421_reset.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-25
 * Assignment: Project 2
 * Description:
 * C program wrapper for fc421_reset syscall.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrappers.h"

int main() {
    long retval = fc421_reset_syscall();
    if (retval) {
        fprintf(stderr, "%ld:%s\n", retval, strerror(retval));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
