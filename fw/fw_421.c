/*
 * File: fw/fw_421.c
 * Author: Matt Manzi <mmanzi1@umbc.edu>
 * Date: 2019-04-09
 * Assignment: Project 2
 * Description:
 * System calls for the Linux kernel to implement a Layer 4 socket-binding
 * firewall and a supplemental file access control system.
 *
 * See include/fw/fw_421.h for more information.
 */
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/in.h> /* for IPPROTO_TCP/UDP macros */
#include <linux/limits.h> /* for PATH_MAX macro */

#include <fw/fw_421.h>

static const uid_t ROOT_UID = 0;

/** define rbtrees **/
struct rb_root ports = RB_ROOT;
struct rb_root files = RB_ROOT;


/** rbtree insert and search function implementations **/

/*
 * port_insert() inserts a new blocked_port struct into the ports rbtree; this
 * implementation is heavily based on the Linux rbtree documentation
 *  - root: pointer to the root of the rbtree to modify
 *  - blockedp: pointer to the struct to insert
 * returns: an integer to indicate the result (0 on success, otherwise failed)
 */
int port_insert(struct rb_root *root, struct blocked_port *blockedp) {

    struct rb_node **new = &(root->rb_node), *parent = NULL;

    /* figure out where to put new node */
    while (*new) {
        struct blocked_port *this = container_of(*new, struct blocked_port,
                                                    node);

        parent = *new;
        if (blockedp->port < this->port) {
            new = &((*new)->rb_left);
        } else if (blockedp->port > this->port) {
            new = &((*new)->rb_right);
        } else {

            /* if we are inserting the same port, check if we're blocking the
            same protocol and direction as what's already stored; otherwise,
            update the flags for this port since we're not blocking more actions
            on the same port number */
            if (this->flags & blockedp->flags) {
                printk("[fw] Port already blocked!\n");
                return -EEXIST;
            } else {
                this->flags |= blockedp->flags;
                return 0;
            }

        }
    }

    /* add new node and rebalance tree */
    rb_link_node(&blockedp->node, parent, new);
    rb_insert_color(&blockedp->node, root);

    return 0;
}

/*
 * port_search() searches the ports rbtree for an entry matching the provided
 * port and flags combination; this implementation is heavily based on the Linux
 * rbtree documentation
 *  - root: pointer to the root of the rbtree to search
 *  - port: port number to search the rbtree for
 *  - flags: protocol and direction flags that should be enabled for the port
 *           number (defined in header)
 * returns: a pointer to the matching struct in the rbtree (or NULL if no match)
 */
struct blocked_port *port_search(struct rb_root *root, unsigned short port,
    unsigned char flag) {

    struct rb_node *node = root->rb_node;

    while (node) {
        struct blocked_port *data = container_of(node, struct blocked_port,
                                                    node);

        if (port < data->port) {
            node = node->rb_left;
        } else if (port > data->port) {
            node = node->rb_right;
        } else if (flag & data->flags) {
            return data;
        } else {
            return NULL;
        }
    }

    return NULL;
}

/*
 * file_insert() inserts a new blocked_file struct into the files rbtree; this
 * implementation is heavily based on the Linux rbtree documentation
 *  - root: pointer to the root of the rbtree to modify
 *  - blockedf: pointer to the struct to insert
 * returns: an integer to indicate the result (0 on success, otherwise failed)
 */
int file_insert(struct rb_root *root, struct blocked_file *blockedf) {

    struct rb_node **new = &(root->rb_node), *parent = NULL;

    /* figure out where to put new node */
    while (*new) {
        struct blocked_file *this = container_of(*new, struct blocked_file,
                                                    node);
        int result = strcmp(blockedf->filename, this->filename);

        parent = *new;
        if (result < 0) {
            new = &((*new)->rb_left);
        } else if (result > 0) {
            new = &((*new)->rb_right);
        } else {
            printk("[fw] File already blocked!\n");
            return -EEXIST;
        }
    }

    /* add new node and rebalance tree */
    rb_link_node(&blockedf->node, parent, new);
    rb_insert_color(&blockedf->node, root);

    return 0;
}

/*
 * file_search() searches the files rbtree for an entry matching the provided
 * filename; this implementation is heavily based on the Linux rbtree
 * documentation
 *  - root: pointer to the root of the rbtree to search
 *  - filename: filename to search the rbtree for
 * returns: a pointer to the matching struct in the rbtree (or NULL if no match)
 */
struct blocked_file *file_search(struct rb_root *root, char *filename) {

    struct rb_node *node = root->rb_node;

    while (node) {
        struct blocked_file *data = container_of(node, struct blocked_file,
                                                    node);
        int result = strcmp(filename, data->filename);

        if (result < 0) {
            node = node->rb_left;
        } else if (result > 0) {
            node = node->rb_right;
        } else{
            return data;
        }
    }

    return NULL;
}


/** helper function implementations **/

/* erase_ports_tree() deletes all the elements in the ports rbtree, but it will
 * not delete its root
 *  - root: pointer to the root of the rbtree to erase
 * returns: a long integer to indicate the result of the erasing (0 for success,
 *          otherwise error)
 * NOTE: due to the way that rbtrees rebalance and operate internally, this
 * method may lead to memory leaks in the kernel.  However, it is not very
 * evident after a bit of testing, even in low memory situations, so it is not a
 * major concern at the moment (5/9/2019).
 */
long erase_ports_tree(void) {
    struct rb_node *node;

    for (node = rb_first(&ports); node; node = rb_next(node)) {
        struct blocked_port *data = container_of(node, struct blocked_port,
                                                    node);

        /* delete an entry */
        if (data) {
            rb_erase(&data->node, &ports);

            /* free memory */
            kfree(data);
            data = NULL;
        }

    }

    return 0;
}

/* erase_files_tree() deletes all the elements in the files rbtree, but it will
 * not delete its root
 *  - root: pointer to the root of the rbtree to erase
 * returns: a long integer to indicate the result of the erasing (0 for success,
 *          otherwise error)
 * NOTE: due to the way that rbtrees rebalance and operate internally, this
 * method may lead to memory leaks in the kernel.  However, it is not very
 * evident after a bit of testing, even in low memory situations, so it is not a
 * major concern at the moment (5/9/2019).
 */
long erase_files_tree(void) {
    struct rb_node *node;

    for (node = rb_first(&files); node; node = rb_next(node)) {
        struct blocked_file *data = container_of(node, struct blocked_file,
                                                    node);

        /* delete an entry */
        if (data) {
            rb_erase(&data->node, &files);

            /* free memory */
            kfree(data->filename);
            data->filename = NULL;
            kfree(data);
            data = NULL;
        }

    }

    return 0;
}


/** syscall implementations **/

/*
 * reset the firewall system to its initial state. This should be done
 * implicitly on boot — that is to say that you should not have to call this
 * system call before setting up the firewall, but that you can use it after
 * boot if you wish to reset the system. Returns 0 on success.
 */
SYSCALL_DEFINE0(fw421_reset) {

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fw] Only root can reset blocked ports!\n");
        return -EPERM;
    }

    /* check if tree is already emtpy */
    if (RB_EMPTY_ROOT(&ports)) {
        return 0;
    }

    return erase_ports_tree();
}

/*
 * add a port to the list that is being blocked, if it is not already in the
 * list. Returns -EEXIST if the port is already blocked, and an appropriate
 * negative error code on any other errors. Returns 0 on success.
 */
SYSCALL_DEFINE3(fw421_block_port, int, proto, int, dir, unsigned short, port) {

    unsigned char flag;
    struct blocked_port *bp;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fw] Only root can block ports!\n");
        return -EPERM;
    }

    /* validate and set protocol to block (default is out) */
    if (proto == IPPROTO_TCP) {
        flag = TCP_OUT;
    } else if (proto == IPPROTO_UDP) {
        flag = UDP_OUT;
    } else {
        printk("[fw] Protocol must be either IPPROTO_TCP or IPPROTO_UDP!\n");
        return -EINVAL;
    }

    /* validate direction to block */
    if (dir != OUTGOING && dir != INCOMING) {
        printk("[fw] Direction must be either %d or %d!\n", OUTGOING, INCOMING);
        return -EINVAL;
    }

    /* adjust flag for direction to block */
    flag = flag << dir;

    /* validate port number */
    if (port < MIN_PORT || port > MAX_PORT) {
        printk("[fw] Port number must be in range [%hu, %hu]!\n", MIN_PORT,
                MAX_PORT);
        return -EINVAL;
    }

    /* alloc space for new blocked port */
    bp = (struct blocked_port *)kmalloc(sizeof(struct blocked_port),GFP_KERNEL);
    if (!bp) {
        printk("[fw] Failed to allocate memory for port block!\n");
        return -ENOMEM;
    }

    /* create the blocked port */
    bp->port = port;
    bp->flags = flag;
    bp->attempts[TCP_OUT - 1] = 0;
    bp->attempts[TCP_IN - 1] = 0;
    bp->attempts[(UDP_OUT >> 4) + 1] = 0;
    bp->attempts[(UDP_IN >> 4) + 1] = 0;

    /* add it to the blocked ports rbtree */
    return port_insert(&ports, bp);
}

/*
 * remove a port from the list that is being blocked, if it is currently on the
 * list. Returns -ENOENT if the port specified is not blocked, and an
 * appropriate negative error code on any other errors. Returns 0 on success.
 */
SYSCALL_DEFINE3(fw421_unblock_port, int, proto, int, dir, unsigned short, port){

    unsigned char flag;
    struct blocked_port *bp;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fw] Only root can unblock ports!\n");
        return -EPERM;
    }

    /* validate and set protocol to block (default is out) */
    if (proto == IPPROTO_TCP) {
        flag = TCP_OUT;
    } else if (proto == IPPROTO_UDP) {
        flag = UDP_OUT;
    } else {
        printk("[fw] Protocol must be either IPPROTO_TCP or IPPROTO_UDP!\n");
        return -EINVAL;
    }

    /* validate direction to block */
    if (dir != OUTGOING && dir != INCOMING) {
        printk("[fw] Direction must be either %d or %d!\n", OUTGOING, INCOMING);
        return -EINVAL;
    }

    /* adjust flag for direction to block */
    flag = flag << dir;

    /* validate port number */
    if (port < MIN_PORT || port > MAX_PORT) {
        printk("[fw] Port number must be in range [%hu, %hu]!\n", MIN_PORT,
                MAX_PORT);
        return -EINVAL;
    }

    /* get blocked port from rbtree */
    bp = port_search(&ports, port, flag);
    if (!bp) {
        printk("[fw] Port not currently blocked!\n");
        return -ENOENT;
    }

    /* if we're unblocking the only flag set for the port, just delete it */
    if (bp->flags == flag) {
        rb_erase(&bp->node, &ports);
        kfree(bp);
        bp = NULL;
        return 0;
    }

    /* otherwise, remove the flag and set the corresponding attempts to 0 */
    bp->flags &= ~flag;
    if (flag > TCP_IN) {
        bp->attempts[(flag >> 4) + 1] = 0;
    } else {
        bp->attempts[flag - 1] = 0;
    }

    return 0;
}

/*
 * query the firewall for the access count of the given port, if it is currently
 * in the list. Returns -ENOENT if the port specified is not blocked, and an
 * appropriate negative error code on any other errors. Returns the non-negative
 * access count on success.
 */
SYSCALL_DEFINE3(fw421_query, int, proto, int, dir, unsigned short, port) {

    unsigned char flag;
    struct blocked_port *bp;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fw] Only root can query blocked ports!\n");
        return -EPERM;
    }

    /* validate and set protocol to block (default is out) */
    if (proto == IPPROTO_TCP) {
        flag = TCP_OUT;
    } else if (proto == IPPROTO_UDP) {
        flag = UDP_OUT;
    } else {
        printk("[fw] Protocol must be either IPPROTO_TCP or IPPROTO_UDP!\n");
        return -EINVAL;
    }

    /* validate direction to block */
    if (dir != OUTGOING && dir != INCOMING) {
        printk("[fw] Direction must be either %d or %d!\n", OUTGOING, INCOMING);
        return -EINVAL;
    }

    /* adjust flag for direction to block */
    flag = flag << dir;

    /* validate port number */
    if (port < MIN_PORT || port > MAX_PORT) {
        printk("[fw] Port number must be in range [%hu, %hu]!\n", MIN_PORT,
                MAX_PORT);
        return -EINVAL;
    }

    /* get blocked port from rbtree */
    bp = port_search(&ports, port, flag);
    if (!bp) {
        printk("[fw] Port not currently blocked!\n");
        return -ENOENT;
    }

    /* get attempts for this flag */
    if (flag > TCP_IN) {
        return bp->attempts[(flag >> 4) + 1];
    } else {
        return bp->attempts[flag - 1];
    }

}

/*
 * reset the file control mechanism to its initial state, unblocking all files.
 * This should be done implicitly on boot — that is to say that you should not
 * have to call this system call before setting up the file control system, but
 * that you can use it after boot if you wish to reset the system. Returns 0 on
 * success.
 */
SYSCALL_DEFINE0(fc421_reset) {

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fc] Only root can reset blocked files!\n");
        return -EPERM;
    }

    /* check if tree is already emtpy */
    if (RB_EMPTY_ROOT(&files)) {
        return 0;
    }

    return erase_files_tree();
}

/*
 * add a file to the list of blocked files, if it is not already in the list.
 * Returns -EEXIST if the file is already blocked, and an appropriate negative
 * error code on any other errors. Returns 0 on success.
 */
SYSCALL_DEFINE1(fc421_block_file, const char __user *, filename) {

    struct blocked_file *bf;
    size_t fn_len;
    char *temp_fn;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fc] Only root can block files!\n");
        return -EPERM;
    }

    /* check filename is not NULL */
    if (!filename) {
        printk("[fc] Filename must not be NULL!\n");
        return -EFAULT;
    }

    /* check filename is an absolute path (first char is /) */
    if (*filename != '/') {
        printk("[fc] Filename must be an absolute path!\n");
        return -EINVAL;
    }

    /* verify we didn't get a bogus filename pointer
     * by checking if its strlen found the same ending point as the first NUL */
    fn_len = strnlen_user(filename, PATH_MAX);

    /* alloc space for new blocked file */
    bf = (struct blocked_file *)kmalloc(sizeof(struct blocked_file),GFP_KERNEL);
    if (!bf) {
        printk("[fc] Failed to allocate memory for file block!\n");
        return -ENOMEM;
    }

    /* create a place to copy in the filename to */
    temp_fn = (char *)kcalloc(fn_len + 1, sizeof(char), GFP_KERNEL);
    if (!temp_fn) {
        printk("[fc] Failed to allocate memory for temp filename storage!\n");
        kfree(bf);
        bf = NULL;
        return -ENOMEM;
    }

    /* copy in the filename */
    if (__copy_from_user(temp_fn, filename, sizeof(char) * (fn_len + 1))) {
        printk("[fc] Unable to copy filename into firewall!\n");
        kfree(temp_fn);
        temp_fn = NULL;
        kfree(bf);
        bf = NULL;
        return -EFAULT;
    }

    /* remove trailing directory slashes from rbtree entries */
    if (temp_fn[fn_len - 2] == '/') {
        temp_fn[fn_len - 2] = '\0';
    }

    bf->filename = temp_fn;
    bf->attempts = 0;

    return file_insert(&files, bf);
}

/*
 * remove a file from the list of blocked files, if it is currently in the list.
 * Returns -ENOENT if the file is not blocked and an appropriate negative error
 * code on any other errors. Returns 0 on success.
 */
SYSCALL_DEFINE1(fc421_unblock_file, const char __user *, filename) {

    struct blocked_file *bf;
    size_t fn_len;
    char *temp_fn;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fc] Only root can unblock files!\n");
        return -EPERM;
    }

    /* check filename is not NULL */
    if (!filename) {
        printk("[fc] Filename must not be NULL!\n");
        return -EFAULT;
    }

    /* check filename is an absolute path (first char is /) */
    if (*filename != '/') {
        printk("[fc] Filename must be an absolute path!\n");
        return -EINVAL;
    }

    /* verify we didn't get a bogus filename pointer
     * by checking if its strlen found the same ending point as the first NUL */
    fn_len = strnlen_user(filename, PATH_MAX);

    /* create a place to copy in the filename to */
    temp_fn = (char *)kcalloc(fn_len + 1, sizeof(char), GFP_KERNEL);
    if (!temp_fn) {
        printk("[fc] Failed to allocate memory for temp filename storage!\n");
        return -ENOMEM;
    }

    /* copy in the filename */
    if (__copy_from_user(temp_fn, filename, sizeof(char) * (fn_len + 1))) {
        printk("[fc] Unable to unblock filename!\n");
        kfree(temp_fn);
        temp_fn = NULL;
        return -EFAULT;
    }

    /* remove trailing directory slashes from rbtree entries */
    if (temp_fn[fn_len - 2] == '/') {
        temp_fn[fn_len - 2] = '\0';
    }

    /* get blocked file from rbtree */
    bf = file_search(&files, temp_fn);
    if (!bf) {
        printk("[fc] File not currently blocked!\n");
        kfree(temp_fn);
        temp_fn = NULL;
        return -ENOENT;
    }

    /* delete the blocked file */
    rb_erase(&bf->node, &files);
    kfree(bf->filename);
    bf->filename = NULL;
    kfree(bf);
    bf = NULL;
    kfree(temp_fn);
    temp_fn = NULL;

    return 0;
}

/*
 * query the file control system for the number of attempted accesses to a
 * blocked file, if it is currently in the list. Returns -ENOENT if the file is
 * not blocked, and an appropriate negative error code on any other errors.
 * Returns the non-negative access count on success.
 */
SYSCALL_DEFINE1(fc421_query, const char __user *, filename) {

    struct blocked_file *bf;
    size_t fn_len;
    char *temp_fn;

    /* validate root user */
    if (current_cred()->uid.val != ROOT_UID) {
        printk("[fc] Only root can query blocked files!\n");
        return -EPERM;
    }

    /* check filename is not NULL */
    if (!filename) {
        printk("[fc] Filename must not be NULL!\n");
        return -EFAULT;
    }

    /* check filename is an absolute path (first char is /) */
    if (*filename != '/') {
        printk("[fc] Filename must be an absolute path!\n");
        return -EINVAL;
    }

    /* verify we didn't get a bogus filename pointer
     * by checking if its strlen found the same ending point as the first NUL */
    fn_len = strnlen_user(filename, PATH_MAX);

    /* create a place to copy in the filename to */
    temp_fn = (char *)kcalloc(fn_len + 1, sizeof(char), GFP_KERNEL);
    if (!temp_fn) {
        printk("[fc] Failed to allocate memory for temp filename storage!\n");
        return -ENOMEM;
    }

    /* copy in the filename */
    if (__copy_from_user(temp_fn, filename, sizeof(char) * (fn_len + 1))) {
        printk("[fc] Unable to unblock filename!\n");
        kfree(temp_fn);
        temp_fn = NULL;
        return -EFAULT;
    }

    /* remove trailing directory slashes from rbtree entries */
    if (temp_fn[fn_len - 2] == '/') {
        temp_fn[fn_len - 2] = '\0';
    }

    /* get blocked file from rbtree */
    bf = file_search(&files, temp_fn);
    if (!bf) {
        printk("[fc] File not currently blocked!\n");
        kfree(temp_fn);
        temp_fn = NULL;
        return -ENOENT;
    }

    kfree(temp_fn);
    temp_fn = NULL;

    return bf->attempts;
}
