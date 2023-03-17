#!/bin/bash

rm *.o .coverage ./target_program 
clang++ -g -fsanitize-coverage=trace-pc-guard ./target_program.c -c
clang++ -fsanitize=address trace-pc-guard.cc  ./target_program.o ./coverage.c -o ./target_program

#tmpdir=$(mktemp -d /tmp/afl-proxy.XXX)
tmpdir="./temp_dir"
rm -r $tmpdir
mkdir $tmpdir
from_proxy="$tmpdir/p2target"
to_proxy="$tmpdir/target2p"
mkfifo "$from_proxy"
mkfifo "$to_proxy"


#./driver EXECUTABLE INPUT_FILE TO_PROXY FROM_PROXY
./driver ./target_program PASS $from_proxy $to_proxy 

#./driver ./target_program PASS $c2a $a2c
#./driver ./target_program FAIL $c2a $a2c

#rm -r $tmpdir
