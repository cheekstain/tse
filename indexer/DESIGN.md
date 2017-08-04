# CS50 Lab 5
## Christina Lu, CS50 Summer 2017

### Indexer Design

The job of the `indexer` is to read the documents in the `page_directory` 
output by the `crawler`, build an *inverted index* mapping from words to 
documents, and write that index to a file. It’s called an *inverted* index 
because it maps from words to documents, the opposite of a document (itself a 
sequence of words).

### Data Structure: Inverted Index

The *inverted index* data structure is implemented using the hashtable, set, 
and counters modules. It is simply a hashtable (that is in turn implemented
by buckets of sets) that maps a `char*` key, the word to be indexed, to a
`counters_t*` which in turn stores `(int, int)` pairs of (DocIDs, count).
Thus, the *index* can be used to map words to both IDs and their corresponding
counts.
