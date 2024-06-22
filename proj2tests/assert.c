/* File: proj2tests/assert.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * Assert functions to make testing smoother.
 */
#include <stdio.h>
#include "assert.h"

char* strassert(int passed) {

    if (passed) {
        return "\033[38;5;47mPassed\033[0m";
    }

    return "\033[38;5;160;1mFailed\033[0m";
}

int passert(int passed) {
    return printf("%s\n", strassert(passed));
}

int assertd(const char *test, int actual, int expected) {
    int passed = (actual == expected);
    printf("[ %s ] %s", strassert(passed), test);
    printf(" (E=%d :: A=%d)\n", expected, actual);
    return passed;
}

int assertl(const char *test, long actual, long expected) {
    int passed = (actual == expected);
    printf("[ %s ] %s", strassert(passed), test);
    printf(" (E=%ld :: A=%ld)\n", expected, actual);
    return passed;
}

int assertlu(const char *test, unsigned long actual, unsigned long expected) {
    int passed = (actual == expected);
    printf("[ %s ] %s", strassert(passed), test);
    printf(" (E=%lu :: A=%lu)\n", expected, actual);
    return passed;
}

int asserts(const char *test, const unsigned char *actual,
            const unsigned char *expected, size_t n) {

    char buffer_actual[n + 1];
    char buffer_expected[n + 1];

    int passed = 1;
    for (size_t i = 0; i < n; i++) {
        if (actual[i] != expected[i]) {
            passed = 0;
        }
    }

    for (size_t i = 0; i < n + 1; i++) {

        if (actual[i] == '\0') {
            buffer_actual[i] = '|';
        } else if (actual[i] == '\n') {
            buffer_actual[i] = '`';
        } else {
            buffer_actual[i] = actual[i];
        }

        if (expected[i] == '\0') {
            buffer_expected[i] = '|';
        } else if (expected[i] == '\n') {
            buffer_expected[i] = '`';
        } else {
            buffer_expected[i] = expected[i];
        }

        if (i == n) {
            buffer_actual[i] = '\0';
            buffer_expected[i] = '\0';
        }

    }

    printf("[ %s ] %s\n", strassert(passed), test);
    printf("  E=%s\n  A=%s\n", buffer_expected, buffer_actual);

    return passed;
}
