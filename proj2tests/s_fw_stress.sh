#!/bin/bash
# File: proj2tests/t_fw_stress.sh
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-05-10
# Assignment: Project 2
# Description:
# Performs 3 stress tests on the firewall system:
# 1) single entry is blocked and immediately unblocked a large number of times
#    to ensure no memory leaks
# 2) block many ports (multiple times) and call reset to see if it's all okay
# 3) add many entries, then remove the entries from the system

# All constants: the higher the number(s), the more stressful the test
PORTS=1000 # for test 1
TIMES_TO_RESET=10 # for test 2
PORTS_TO_RESET=2000 # for test 2
NUM_PORTS=300 # for test 3


# lg(): compute the log-base-2 of a number
function lg() {
    local x=0
    for (( y=$1-1; $y > 0; y >>= 1 )) ; do
        let x=$x+1
    done
    echo $x
}


## stress test 1 ##

expected=$(( 2 * $PORTS * $(lg $PORTS) ))
start=$(date +%s)
for (( i=1; i<$PORTS; i++ )); do
    # echo "$i"
    ${DRIVERS}fw421_block_port tcp out "$i"
    ${DRIVERS}fw421_block_port tcp in "$i"
    ${DRIVERS}fw421_block_port udp out "$i"
    ${DRIVERS}fw421_block_port udp in "$i"
    ${DRIVERS}fw421_unblock_port tcp out "$i"
    ${DRIVERS}fw421_unblock_port tcp in "$i"
    ${DRIVERS}fw421_unblock_port udp out "$i"
    ${DRIVERS}fw421_unblock_port udp in "$i"
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
    for (( j=1; j<$PORTS_TO_RESET; j++ )); do
        ${DRIVERS}fw421_block_port tcp out "$j"
        ${DRIVERS}fw421_block_port tcp in "$j"
        ${DRIVERS}fw421_block_port udp out "$j"
        ${DRIVERS}fw421_block_port udp in "$j"
    done
    ${DRIVERS}fw421_reset
    ./assertl "Completed stress test 2 (part $i/$TIMES_TO_RESET)" "0" "0"
done
end=$(date +%s)
actual=$(( $end - $start ))
./assertl "Completed stress test 2" "$actual" "$actual"


## stress test 3 ##

# block a bunch of ports
expected=$(( $(lg $NUM_PORTS) ))
start=$(date +%s)
for (( i=1; i<$NUM_PORTS; i++ )); do
    ${DRIVERS}fw421_block_port tcp out "$i"
    ${DRIVERS}fw421_block_port tcp in "$i"
    ${DRIVERS}fw421_block_port udp out "$i"
    ${DRIVERS}fw421_block_port udp in "$i"
done
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Blocked $NUM_PORTS ports" "$actual" "$actual"

# unblock the ports
expected=$(( $(lg $NUM_PORTS) ))
start=$(date +%s)
for (( i=1; i<$NUM_PORTS; i++ )); do
    ${DRIVERS}fw421_unblock_port tcp out "$i"
    ${DRIVERS}fw421_unblock_port tcp in "$i"
    ${DRIVERS}fw421_unblock_port udp out "$i"
    ${DRIVERS}fw421_unblock_port udp in "$i"
done
end=$(date +%s)
actual=$(( $end - $start ))
if [ "$actual" -le "$expected" ]; then
    expected=$actual
fi
./assertl "Unblocked $NUM_PORTS ports" "$actual" "$actual"

# reset
${DRIVERS}fw421_reset
