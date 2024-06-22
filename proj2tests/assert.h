/* File: proj2tests/assert.h
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * Header file for assert functions to make testing smoother.
 */
#ifndef ASSERT_H
#define ASSERT_H

char* strassert(int passed);

int passert(int passed);

int assertd(const char *test, int actual, int expected);

int assertl(const char *test, long actual, long expected);

int assertlu(const char *test, unsigned long actual, unsigned long expected);

int asserts(const char *test, const unsigned char *actual,
                const unsigned char *expected, size_t n);

#endif
