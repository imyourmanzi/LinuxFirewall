#!/bin/bash
# File: proj2tests/t_fc_sys.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-04-25
# Assignment: Project 2
# Description:
# Tests successful operation of the system calls for file access control

# iterate all file access control tests
tests=( fc421_reset fc421_block_file fc421_query fc421_unblock_file )
for fc in "${tests[@]}"; do

    # run a test (reset gets a param too but it's okay)
    $DRIVERS$fc /tmp/block_me 2> tmp
    actual="$?"
    ./assertl "$fc" "$actual" "0"
    if [ -s tmp ]; then
        echo -e "\033[38;5;217mError Occurred\033[0m"
        echo -n " => "
        cat tmp
    fi
    rm tmp

done
