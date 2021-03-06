#!/bin/sh
# clear prior results, and mark start time
date > test-results
echo >> test-results
# run tests, record output
for i in  ../U*20*/*.cnf  ../U*50-218*/*.cnf
do
    echo $i >> test-results
    # this interleaves output from cnf program, and timing information
    # time takes an optional format string which could be used to clean
    # this up.
    # this line works on linux, not mac, apples time is not as clever
    time  -o test-results -a ./cnf $i  >> test-results
    echo >> test-results
    echo >> test-results
done

# mark stop time
date >> test-results
