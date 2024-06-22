/*
 * File: include/fw/fw_421.h
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-15
 * Assignment: Project 2
 * Description:
 * Header file for Linux kernel to implement a Layer 4 socket-binding firewall
 * and a supplemental file access control system.
 *
 * For proto in all of the firewall related calls, the value will be one of the
 * symbolic constants IPPROTO_TCP or IPPROTO_UDP as defined in the standard C
 * library. Outgoing implies a dir of 0, where incoming implies a dir of 1.
 */
#ifndef FW_421_H
#define FW_421_H

#include <linux/rbtree.h>


/** constants **/

/* directions */
#define OUTGOING 0
#define INCOMING 1

/* port number bounds */
#define MIN_PORT 1
#define MAX_PORT 65535

/*
 * flags for marking what kind of port is blocked
 *
 * these flags can alse be used to access their respective "attempt" counts in
 * the `attempts` array:
 *  - TCP: subtract 1 from the flag for the index
 *  - UDP: shift right 4 bits, add 1 for the index
 * If given an flag built from variables, check if `flag > TCP_IN` to know which
 * procedure to follow.
 */
#define TCP_OUT 0x01
#define TCP_IN  0x02
#define UDP_OUT 0x10
#define UDP_IN  0x20


/** storage data structure declarations **/

/*
 * struct to define a blocked port
 * port: the port number to block
 * flags: a combination of one or more of the above flags to indicate blocked
 *        protocol(s) and direction(s)
 */
struct blocked_port {
    struct rb_node node;
    unsigned short port;
    unsigned char flags;
    long attempts[4];
};

/*
 * struct to define a blocked file
 * filename: C-string of the absolute path of the file that is to be blocked
 */
struct blocked_file {
    struct rb_node node;
    char *filename;
    long attempts;
};

extern struct rb_root ports;
extern struct rb_root files;


/** rbtree insert and search function declarations **/
int port_insert(struct rb_root *root, struct blocked_port *blockedp);
struct blocked_port *port_search(struct rb_root *root,
    unsigned short port, unsigned char flag);

int file_insert(struct rb_root *root, struct blocked_file *blockedf);
struct blocked_file *file_search(struct rb_root *root, char *filename);

/** helper function declarations **/
long erase_ports_tree(void);
long erase_files_tree(void);

#endif  /* FW_421_H */
