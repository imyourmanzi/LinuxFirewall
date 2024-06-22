#!/bin/bash
# File: proj2tests/t_fw_sys.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-04-25
# Assignment: Project 2
# Description:
# Tests successful operation of the system calls for the firewall

# iterate all firewall tests
tests=( fw421_reset fw421_block_port fw421_query fw421_unblock_port )
for fw in "${tests[@]}"; do

    # run a test (reset gets params too but it's okay)
    $DRIVERS$fw tcp in 4444 2> tmp
    actual="$?"
    ./assertl "$fw" "$actual" "0"
    if [ -s tmp ]; then
        echo -e "\033[38;5;217mError Occurred\033[0m"
        echo -n " => "
        cat tmp
    fi
    rm tmp

done
