# Linux Firewall Project

A project to build a simple firewall and file access control system into the 4.x Linux kernel (for x86 architectures).

At a high level, the new functionality adds a few system calls directly to the kernel. They're exposed only to the `root` user through CLI commands. The commands allow `root` to block, unblock, and check the status of TCP/UDP ports and any file on the filesystem.

## Here's What's Interesting

- [**Project Documentation**](/proj2docs/) - this documentation folder contains the design docs for the project, including both a preliminary exploration through the kernel and the final design explanation.
- [**Tests**](/proj2tests/) - the tests folder contains its own README for instructions on running the numerous and detailed test bench that I custom wrote for this project.  See below for a sample test run.
- [**Code**](/proj2driver/) - this is where some of the custom code lives, mostly net-new code that was added to the kernel, not including modifications of existing files or functions.  You can also check out these files for more interesting code points:
   - [fw_421.c](/fw/fw_421.c) - system call implementations
   - [namei.c](/fs/namei.c) - filesystem controls
   - [socket.c](/net/socket.c) - firewall controls
   - Other modifications are noted in the Project Documentation

### Sample Test Run

![Sample test run](/proj2docs/sample_test_run.gif)

## Background

This project was the second of two in my college operating systems course (you'll notice `proj2` and `421` baked into many identifiers).

This project was one of the most difficult, yet rewarding, I've ever worked on. It gave me a whole new appreciation for software and operating systems, that latter of which I'd like to have a career in one day. Despite that I scored over 100% on this assignment (after extra credit and a curve), I still have one failing test in my test suite that I intend to solve some day.
