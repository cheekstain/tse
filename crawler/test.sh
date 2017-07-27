#!/bin/bash
# 
# test.sh - perform crawler testing
#
# usage: test.sh
#
# input: none
# output: writes files as results from tests
#
# Christina Lu, July 2017

# makes sure directory does not exist
rm -rf test_data
mkdir test_data

# parameter test, check error messages
./crawler
./crawler hello my name is christina
./crawler http://www.cs.dartmouth.edu/ ./test_data/ 0
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/index.html fakedir 0
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/index.html ./test_data/ -5

# test 0: cheekstain playground depth 0
mkdir test_data/test0/
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/index.html ./test_data/test0/ 0

# test 1: cheekstain playground depth 4
mkdir test_data/test1/
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/index.html ./test_data/test1/ 4

# test 2: cheekstain playground different seed, depth 2
# runs into J.html which does not exist, prints to stderr and continues
mkdir test_data/test2/
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/G.html ./test_data/test2/ 2

# test 3: wikipedia, depth 1
mkdir test_data/test3/
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./test_data/test3/ 1







