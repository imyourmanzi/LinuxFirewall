#!/bin/bash
# File: proj2tests/t_fc_dir_block.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-01
# Assignment: Project 2
# Description:
# Tests that a single directory can be blocked, that its subfiles are blocked,
# and that directory operations do not work on it.

block_dir="$(pwd)/answers_to_life"
block_subfile="${block_dir}/nothing"

# create the dir and subfile
mkdir "$block_dir"
echo "yee haw ;)" > "$block_subfile"


# block dir (without trailing '/')
# -> try `stat` blocked dir
# '-> without and with the trailing '/'
# -> try `ls -l` blocked dir (without '/')
# -> try `cat` blocked subfile
# -> try `rmdir` (without '/')
# unblock dir (without trailing '/')
# block dir (with trailing '/')
# -> try `stat` blocked dir
# '-> without and with the trailing '/'

${DRIVERS}fc421_block_file "$block_dir"
./assertl "Block directory (without trailing '/')" "$?" "0"

stat "$block_dir"
./assertl "\`stat\` directory (without trailing '/') fails" "$?" "1"

stat "${block_dir}/"
./assertl "\`stat\` directory (with trailing '/') fails" "$?" "1"

ls -l "$block_dir"
./assertl "\`ls -l\` of blocked directory fails" "$?" "2"

cat "$block_subfile" > /dev/null
./assertl "\`cat\` file in blocked directory succeeds" "$?" "0"

${DRIVERS}fc421_unblock_file "$block_dir"
${DRIVERS}fc421_block_file "${block_dir}/"
./assertl "Block directory (with trailing '/')" "$?" "0"

stat "$block_dir"
./assertl "\`stat\` directory (without trailing '/') fails" "$?" "1"

stat "${block_dir}/"
./assertl "\`stat\` directory (with trailing '/') fails" "$?" "1"


# -> block a non-existent directory
# -> try `mkdir` non-existent directory
future_dir="$(pwd)/oops"
${DRIVERS}fc421_block_file "$future_dir"
./assertl "Block non-existent directory" "$?" "0"

mkdir "$future_dir"
./assertl "\`mkdir\` non-existent directory fails" "$?" "1"


# unblock dirs and files
${DRIVERS}fc421_unblock_file "$future_dir"
${DRIVERS}fc421_unblock_file "$block_dir/"

# reset
${DRIVERS}fc421_reset

# remove now-unblocked dirs and files
rm -rf "$future_dir"
rm "$block_subfile"
rmdir "$block_dir"
