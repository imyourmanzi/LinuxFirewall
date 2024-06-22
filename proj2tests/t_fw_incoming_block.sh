#!/bin/bash
# File: proj2tests/t_fw_incoming_block.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-07
# Assignment: Project 2
# Description:
# Block port 2019 incoming to localhost, trying both TCP and UDP

# block TCP port
${DRIVERS}fw421_block_port tcp in 2019


## should fail ##

# attempt to bind TCP
err=$(${DRIVERS}bind 2019 tcp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP4 bind fails" "$err" "bind:1"
err=$(${DRIVERS}bind 2019 tcp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP6 bind fails" "$err" "bind:1"


## should work ##

# attempt to bind UDP
${DRIVERS}bind 2019 udp 4
./assertl "UDP4 bind succeeds" "$?" "0"

# TODO: test a basic bind and block (which should not fail)
# if we use an already running SSH server, that's already bound so our block
# should not affect new connections
# read -p "Make sure you have an SSH connection established (Enter to continue)"
#${DRIVERS}fc421_block_port tcp in 22
# read -p "Check your current SSH connection (Enter to continue)"
#${DRIVERS}fc421_unblock_port tcp in 22


# unblock TCP port and block UDP port
${DRIVERS}fw421_unblock_port tcp in 2019
${DRIVERS}fw421_block_port udp in 2019


## should fail ##

# attempt to bind UDP
err=$(${DRIVERS}bind 2019 udp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP4 bind fails" "$err" "bind:1"
err=$(${DRIVERS}bind 2019 udp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP6 bind fails" "$err" "bind:1"


## should work ##

# attempt to bind TCP
${DRIVERS}bind 2019 tcp 4
./assertl "TCP4 bind succeeds" "$?" "0"


# reset firewall
${DRIVERS}fw421_reset
