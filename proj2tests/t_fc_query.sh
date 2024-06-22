#!/bin/bash
# File: proj2tests/t_fc_query.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-12
# Assignment: Project 2
# Description:
# Tests that query returns the proper number of attempts for a file access
ATTEMPTS=10

block_file="$(pwd)/block_me"

# create a file and block it
echo "You can't block me!" > "$block_file"
${DRIVERS}fc421_block_file "$block_file"

# try to stat the file
for (( i=0; i<$ATTEMPTS; i++ )); do
    stat "$block_file"
done

# check the query
att=$(${DRIVERS}fc421_query "$block_file" | awk '{ print $1 }')
./assertl "Query returns expected number of attempts" "$att" "$ATTEMPTS"

# unblock the files
${DRIVERS}fc421_unblock_file "$block_file"

# reset
${DRIVERS}fc421_reset

# remove now-unblocked file
rm "$block_file"
