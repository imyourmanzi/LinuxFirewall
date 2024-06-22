#!/bin/bash
# File: proj2tests/t_fw_single_port_multi.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-06
# Assignment: Project 2
# Description:
# Block a single port in both protocols and both directions at the same time,
# and attempt to make the underlying rbtree add and remove flags properly

# block all
${DRIVERS}fw421_block_port tcp in 2019
./assertl "Block TCP in" "$?" "0"
${DRIVERS}fw421_block_port udp in 2019
./assertl "Block UDP in" "$?" "0"
${DRIVERS}fw421_block_port tcp out 2019
./assertl "Block TCP out" "$?" "0"
${DRIVERS}fw421_block_port udp out 2019
./assertl "Block UDP out" "$?" "0"

# unblock a few
${DRIVERS}fw421_unblock_port udp in 2019
./assertl "Unblock UDP in" "$?" "0"
${DRIVERS}fw421_unblock_port tcp in 2019
./assertl "Unblock TCP in" "$?" "0"

# reblock a few
${DRIVERS}fw421_block_port tcp in 2019
./assertl "Reblock TCP in" "$?" "0"
${DRIVERS}fw421_block_port udp in 2019
./assertl "Reblock UDP in" "$?" "0"

# unblock all
${DRIVERS}fw421_unblock_port tcp in 2019
./assertl "Unblock TCP in" "$?" "0"
${DRIVERS}fw421_unblock_port udp in 2019
./assertl "Unblock UDP in" "$?" "0"
${DRIVERS}fw421_unblock_port tcp out 2019
./assertl "Unblock TCP out" "$?" "0"
${DRIVERS}fw421_unblock_port udp out 2019
./assertl "Unblock UDP out" "$?" "0"

# reset firewall
${DRIVERS}fw421_reset
