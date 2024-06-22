#!/bin/bash
# File: proj2tests/t_fc_stress.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-08
# Assignment: Project 2
# Description:
# Performs 3 stress tests on the file access control system:
# 1) single entry is blocked and immediately unblocked a large number of times
#    to ensure no memory leaks
# 2) block many files (multiple times) and call reset to see if it's all okay
# 3) add many entries, attempt to access a file that is not blocked and then
#    remove the entries from the system

# All constants: the higher the number(s), the more stressful the test
ITERATIONS=50 # for test 1
TIMES_TO_RESET=10 # for test 2
FILES_TO_RESET=10 # for test 2
NUM_FILES=300 # for test 3


# lg(): compute the log-base-2 of a number
function lg() {
    local x=0
    for (( y=$1-1; $y > 0; y >>= 1 )) ; do
        let x=$x+1
    done
    echo $x
}


## stress test 1 ##

expected=$(( 2 * $ITERATIONS * $(lg $ITERATIONS) ))
start=$(date +%s)
for (( i=0; i<$ITERATIONS; i++ )); do
    ${DRIVERS}fc421_block_file "/block_$i"
    ${DRIVERS}fc421_unblock_file "/block_$i"
done
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Completed stress test 1" "$actual" "$actual"


## stress test 2 ##

start=$(date +%s)
for (( i=1; i<=$TIMES_TO_RESET; i++ )); do
    for (( j=0; j<$FILES_TO_RESET; j++ )); do
        ${DRIVERS}fc421_block_file "/block_$i_$j"
    done
    ${DRIVERS}fc421_reset
    ./assertl "Completed stress test 2 (part $i/$TIMES_TO_RESET)" "0" "0"
done
end=$(date +%s)
actual=$(( $end - $start ))
./assertl "Completed stress test 2" "$actual" "$actual"


## stress test 3 ##

# block a bunch of files
expected=$(( $(lg $NUM_FILES) ))
start=$(date +%s)
for (( i=0; i<$NUM_FILES; i++ )); do
    ${DRIVERS}fc421_block_file "/block_$i"
done
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Blocked $NUM_FILES files" "$actual" "$actual"

# cat the hostname file
expected=$(( $(lg $NUM_FILES) ))
start=$(date +%s)
cat /etc/hostname
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Accessed unrelated file" "$actual" "$actual"

# unblock the files
expected=$(( $(lg $NUM_FILES) ))
start=$(date +%s)
for (( i=0; i<$NUM_FILES; i++ )); do
    ${DRIVERS}fc421_unblock_file "/block_$i"
done
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Unblocked $NUM_FILES files" "$actual" "$actual"

# reset
${DRIVERS}fc421_reset
