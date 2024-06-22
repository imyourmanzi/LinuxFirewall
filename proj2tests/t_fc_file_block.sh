#!/bin/bash
# File: proj2tests/t_fc_file_block.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-04-25
# Assignment: Project 2
# Description:
# Tests that a single file can be blocked through direct access, sym, and hard
# links, and that file operations do not work on it.

block_file="$(pwd)/block_me"

# create a file to block
echo "You can't block me!" > "$block_file"


# create symlink
# block file
# -> try `cat`
# -> try `cat` from relative path
# -> try create hard link
# -> try `ls -l $block_file`
# -> try `stat`
# -> try `cat` sym_linked file
# -> try `rm`

ln -s "$block_file" "$(pwd)/sym_linked"
./assertl "Create symlink" "$?" "0"

${DRIVERS}fc421_block_file "$block_file"
./assertl "Block file" "$?" "0"

cat "$block_file"
./assertl "\`cat\` blocked file fails" "$?" "1"

cat block_me
./assertl "\`cat\` blocked file (relative path) fails" "$?" "1"

ln "$block_file" "$(pwd)/hard_linked"
./assertl "Create hard link fails" "$?" "1"

ls -l "$block_file"
./assertl "\`ls -l\` of blocked file fails" "$?" "2"

stat "$block_file"
./assertl "\`stat\` blocked file fails" "$?" "1"

cat "$(pwd)/sym_linked"
./assertl "\`cat\` symlink fails" "$?" "1"

rm "$block_file"
./assertl "\`rm\` blocked file fails" "$?" "1"


# -> block a non-existent file
# -> try `touch` non-existent file
future="$(pwd)/neveragain"
${DRIVERS}fc421_block_file "$future"
./assertl "Block non-existent file" "$?" "0"

touch "$future"
./assertl "\`touch\` non-existent file fails" "$?" "1"


# unblock the files
${DRIVERS}fc421_unblock_file "$block_file"
./assertl "Unblock file" "$?" "0"
${DRIVERS}fc421_unblock_file "$future"
./assertl "Unblock non-existent file" "$?" "0"

# reset
${DRIVERS}fc421_reset

# remove now-unblocked file and its soft link
rm -f "$future"
rm -f "$(pwd)/hard_linked"
rm "$(pwd)/sym_linked"
rm "$block_file"
