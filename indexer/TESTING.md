# CS50 Lab 4
## Christina Lu, CS50 Summer 2017

### Indexer Testing

#### Indexer.c Testing 

Using `indexer.c`, the indexer can be tested for reading from a directory
of crawler files and building an *index* from that. It can first be tested
from the command line for usage errors: directories that are invalid, files
that can't be opened or do not exist, or incorrect arguments, which will all 
print the corrresponding error message to stderr.

Then, the index file it creates can be checked manually against the crawler
files it is called on. The indexer is tested on the [user playground](http://old-www.cs.dartmouth.edu/~cheekstain/tse/).

#### Indextest.c Testing

Using `indextest.c`, the indexer can be tested for reading from an index
file, producing an index structure, and reprinting the exact same contents
to a new file. The two files can then be sorted and compared to each other 
to ensure that the methods work properly.

The arguments are only checked for the correct number, but the function will
exit if it is given files that can't be opened.

#### make test

A tests of both `indexer.c` and `indextest.c` can be called with `make test` 
which runs the script `test.sh`. 

It first checks invalid arguments.

It then uses the crawler to build files within a given directory, then has 
`index.c` read from the files within the directories, create an index, and
print to a given output file. 

`indextest.c` then uses the file that was just produced, builds an index 
from that, and reprints the contents to a new file. Both files will be sorted
then checked against each other to ensure that they work properly.

### Limitations

The indexer has not been tested on very large text files, which may cause
trouble with the load factor of the index. 
