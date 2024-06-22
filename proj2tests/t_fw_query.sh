#!/bin/bash
# File: proj2tests/t_fw_query.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-12
# Assignment: Project 2
# Description:
# Tests that query returns the proper number of attempts for a set of port
# accesses (in/out TCP/UDP)
ATTEMPTS=10

# block a port
${DRIVERS}fw421_block_port tcp out 2019
${DRIVERS}fw421_block_port tcp in 2019
${DRIVERS}fw421_block_port udp out 2019
${DRIVERS}fw421_block_port udp in 2019

# try to use the port
for (( i=0; i<$ATTEMPTS; i++ )); do
    ${DRIVERS}bind 2019 tcp 4
    ${DRIVERS}connect 2019 tcp 4
    ${DRIVERS}bind 2019 udp 4
    ${DRIVERS}connect 2019 udp 4
done

# check the query
att=$(${DRIVERS}fw421_query tcp out 2019 | awk '{ print $1 }')
./assertl "Query returns expected no. of attempts (TCP out)" "$att" "$ATTEMPTS"
att=$(${DRIVERS}fw421_query tcp in 2019 | awk '{ print $1 }')
./assertl "Query returns expected no. of attempts (TCP in)" "$att" "$ATTEMPTS"
att=$(${DRIVERS}fw421_query udp out 2019 | awk '{ print $1 }')
./assertl "Query returns expected no. of attempts (UDP out)" "$att" "$ATTEMPTS"
att=$(${DRIVERS}fw421_query udp in 2019 | awk '{ print $1 }')
./assertl "Query returns expected no. of attempts (UDP in)" "$att" "$ATTEMPTS"

# unblock the port
${DRIVERS}fw421_unblock_port tcp out 2019
${DRIVERS}fw421_unblock_port tcp in 2019
${DRIVERS}fw421_unblock_port udp out 2019
${DRIVERS}fw421_unblock_port udp in 2019

# reset
${DRIVERS}fc421_reset
