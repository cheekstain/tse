# CS50 Lab 6
## Christina Lu, CS50 Summer 2017

### Querier

The *querier* crawls a website and retrieves webpages starting with a
specified URL. It is a standalone program that reads the index
file produced by the TSE Indexer, and page files produced by the TSE Querier,
and answers search queries submitted via stdin.

### Usage

The *querier*, implemented in `querier.c`, contains static functions 
for looking at counters, dealing with queries, and processing them.

### Implementation

See [IMPLEMENTATION](IMPLEMENTATION.md) for details about implementation.

### Assumptions

Input URLs and directories must both end with a `\`. Words have a character 
limit, and the hashtable has 200 slots, but both are likely not do have
any issues unless there is massive amounts of data.

### Compilation

To compile, simply `make`.

To unit test, `make unit`.
