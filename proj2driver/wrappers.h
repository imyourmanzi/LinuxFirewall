/*
 * File: proj2driver/wrappers.h
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * Header file for wrapper for fw system calls.
 */
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_fw421_reset        335
#define __NR_fw421_block_port   336
#define __NR_fw421_unblock_port 337
#define __NR_fw421_query        338
#define __NR_fc421_reset        339
#define __NR_fc421_block_file   340
#define __NR_fc421_unblock_file 341
#define __NR_fc421_query        342

long fw421_reset_syscall(void) {
    syscall(__NR_fw421_reset);
    long e = errno;
    return e;
}

long fw421_block_port_syscall(int proto, int dir, unsigned short port) {
    syscall(__NR_fw421_block_port, proto, dir, port);
    long e = errno;
    return e;
}

long fw421_unblock_port_syscall(int proto, int dir, unsigned short port) {
    syscall(__NR_fw421_unblock_port, proto, dir, port);
    long e = errno;
    return e;
}

long fw421_query_syscall(int proto, int dir, unsigned short port) {
    long attempts = syscall(__NR_fw421_query, proto, dir, port);
    long e = errno;
    if (attempts < 0) {
        return -e;
    }
    return attempts;
}

long fc421_reset_syscall(void) {
    syscall(__NR_fc421_reset);
    long e = errno;
    return e;
}

long fc421_block_file_syscall(const char *filename) {
    syscall(__NR_fc421_block_file, filename);
    long e = errno;
    return e;
}

long fc421_unblock_file_syscall(const char *filename) {
    syscall(__NR_fc421_unblock_file, filename);
    long e = errno;
    return e;
}

long fc421_query_syscall(const char *filename) {
    long attempts = syscall(__NR_fc421_query, filename);
    long e = errno;
    if (attempts < 0) {
        return -e;
    }
    return attempts;
}
