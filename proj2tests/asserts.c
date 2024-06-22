/*
 * File: proj1tests/ktests/asserts.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-18
 * Assignment: Project 2
 * Description:
 * C program wrapper for asserts library function.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <test> <actual> <expected>\n", argv[0]);
        return -1;
    }

    return asserts(argv[1], (unsigned char *)argv[2], (unsigned char *)argv[3], strlen(argv[3]));
}
