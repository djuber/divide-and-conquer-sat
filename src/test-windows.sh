#!/bin/sh

# clear prior results, and mark start time
date > test-results.win
echo >> test-results.win

# run tests, record output
for i in  ../U*/*.cnf 
do
    echo $i >> test-results.win
    # this interleaves output from cnf program, and timing information
    # time takes an optional format string which could be used to clean
    # this up.
    # use wine, the windows emulator, to test PE executable
    time -o test-results.win -a wine ./cnf.exe $i  >> test-results.win
    echo >> test-results.win
    echo >> test-results.win
done

# mark stop time
date >> test-results.win
