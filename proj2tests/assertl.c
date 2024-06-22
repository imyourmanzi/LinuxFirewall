/*
 * File: proj1tests/ktests/assertl.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-18
 * Assignment: Project 2
 * Description:
 * C program wrapper for assertl library function.
 */
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <test> <actual> <expected>\n", argv[0]);
        return -1;
    }

    unsigned long actual = strtoul(argv[2], NULL, 10);
    unsigned long expected = strtoul(argv[3], NULL, 10);

    return assertl(argv[1], actual, expected);
}
