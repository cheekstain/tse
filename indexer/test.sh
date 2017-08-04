#!/bin/bash
# 
# test.sh - perform indexer testing
#
# usage: test.sh
#
# input: none
# output: writes files as results from tests
#
# Christina Lu, August 2017

# makes sure directory does not exist
printf "making new directory data/\n"
make clean
rm -rf data
mkdir data

# parameter test, check error messages
printf "\n\nmaking indexer and indextest:\n"
make all

printf "\n\ntesting invalid parameters, error messages will be printed:\n"
./indexer
./indextest
./indexer hello my name is christina
./indexer ./test_data/ filename

# use crawler to create files within ./data/
# will print that J.html does not exist
printf "\n\nusing crawler to create files within data/, will print J.html error:\n"
cd ../crawler
make
./crawler http://old-www.cs.dartmouth.edu/~cheekstain/tse/index.html ../indexer/data/ 5

# use indexer to read those files and create index file
printf "\n\ntesting indexer, writing to data/old_file...\n"
cd ../indexer
./indexer ./data/ ./data/old_file

# use indextest to read the old file, create index, and write a new file
printf "\n\ntesting indextest, writing from data/old_file to data/new_file...\n"
./indextest ./data/old_file ./data/new_file

# sorts and compares files
printf "\n\nsorting files...\n"
sort ./data/old_file -o ./data/old_file
sort ./data/new_file -o ./data/new_file

printf "\n\ncomparing files, prints result:\n"
if cmp -s ./data/old_file ./data/new_file
then
   echo "The files match"
else
   echo "The files are different"
fi
