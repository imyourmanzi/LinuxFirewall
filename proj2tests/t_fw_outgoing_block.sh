#!/bin/bash
# File: proj2tests/t_fw_outgoing_block.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-08
# Assignment: Project 2
# Description:
# Block port 2019 outgoing to localhost, trying both TCP and UDP

# block TCP port
${DRIVERS}fw421_block_port tcp out 2019


## should fail ##

# attempt to connect TCP
err=$(${DRIVERS}connect 2019 tcp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP4 connect fails" "$err" "connect:1"
err=$(${DRIVERS}connect 2019 tcp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP6 connect fails" "$err" "connect:1"

# TODO: attempt to sendmsg TCP
err=$(${DRIVERS}sendmsg 2019 tcp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP4 sendmsg fails" "$err" "sendmsg:1"
err=$(${DRIVERS}sendmsg 2019 tcp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "TCP6 sendmsg fails" "$err" "sendmsg:1"


## should work ##

# attempt to connect UDP
${DRIVERS}connect 2019 udp 4
./assertl "UDP4 connect succeeds" "$?" "0"

# attempt to sendto UDP
${DRIVERS}sendto 2019 4
./assertl "UDP4 sendto succeeds" "$?" "0"

# attempt to sendmsg UDP
${DRIVERS}sendmsg 2019 udp 4
./assertl "UDP4 sendmsg succeeds" "$?" "0"

# TODO: test a basic connect, block and send (which should not fail)
#${DRIVERS}connect 22 tcp 4
#${DRIVERS}fc421_block_port tcp out 22
#${DRIVERS}send 22
#./assertl "TCP4 send succeeds (after connect then block)" "$?" "0"
#${DRIVERS}fc421_unblock_port tcp out 22


# unblock TCP port and block UDP port
${DRIVERS}fw421_unblock_port tcp out 2019
${DRIVERS}fw421_block_port udp out 2019


## should fail ##

# attempt to connect UDP
err=$(${DRIVERS}connect 2019 udp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP4 connect fails" "$err" "connect:1"
err=$(${DRIVERS}connect 2019 udp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP6 connect fails" "$err" "connect:1"

# attempt to sendto UDP
err=$(${DRIVERS}sendto 2019 4 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP4 sendto fails" "$err" "sendto:1"
err=$(${DRIVERS}sendto 2019 6 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP6 sendto fails" "$err" "sendto:1"

# attempt to sendmsg UDP
err=$(${DRIVERS}sendmsg 2019 udp 4 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP4 sendmsg fails" "$err" "sendmsg:1"
err=$(${DRIVERS}sendmsg 2019 udp 6 2>&1 | cut -d ':' -f 1-2)
./asserts "UDP6 sendmsg fails" "$err" "sendmsg:1"


# reset firewall
${DRIVERS}fw421_reset
