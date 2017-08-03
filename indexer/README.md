# CS50 Lab 5
## Christina Lu, CS50 Summer 2017

### Indexer & Indextest

The TSE *indexer* is a standalone program that reads the document files
produced by the TSE crawler, builds an index, and writes that index to a 
file. Its companion, the *index tester*, loads an index file produced by the 
indexer and saves it to another file.

### Usage

The *indexer* is implemented as an `indexer.c` file. The program implements
methods from the common library's `index.c`. It takes a page directory and 
filename. To compile, simply `make indexer`.

The *index tester* is implemented as an `indextest.c` file. The program also
implements methods from the common library's `index.c` as well as `pagedir.c`.
It takes an old file name to read from and a new file name to write to. To
compile, simply `make indextest`.

Both can be compiled using `make` or `make all`. 

### Implementation

See [IMPLEMENTATION](IMPLEMENTATION.md) for details about implementation.

### Assumptions

Input directories must both end with a `\`. Excessively long file IDs 
(upwards of 10 digits) will not be accepted.
