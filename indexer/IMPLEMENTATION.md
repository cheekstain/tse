# CS50 Lab 5
## Christina Lu, CS50 Summer 2017

### Indexer Implementation

The *indexer* is implemented with `indexer.c`, the bulk of which uses 
functions from the common library's `index.c` module. It utilizes the
`inverted index` data structure, which is discussed in the [DESIGN.md](DESIGN.md)

### indexer.c

Most functions that are used by `indexer.c` are in the common library's 
`index.h`, however, the `indexer.c` program actually initiliazes and executes
the indexer.

`main()` takes parameters and checks them to make sure that they are valid and
acceptible by the program. Then, it initializes the actual indexing by calling
`start_index()`. 

`start_index()` takes the `page_directory` and `filename` and begins the
indexing itself. It first initiliazes the index data structure and calls
`index_build()` which builds the indexer from the given directory. Then it 
calls `index_save()` from the `index.c` module, which writes the content
of the data structure into the acceptable format into the file given by
`filename`. Finally, it cleans up after itself by calling `index_delete()`.

`index_build()` is the only other function within `indexer.c`. It takes a 
`page_directory` and loops through all the files within it, calling 
`index_page()` on each and moving on until all the files have been read
and saved to the index.

### index.c

The `index.c` module within the common library contains most of the functions
that is used by the *indexer*.

`index_new()` creates a new index data structure, with the given number of 
slots. It returns the index upon success, or exits with '1' if it fails with
allocation.

`index_insert()` inserts a key and counter pair to the given index. It 
normalizes the word before insertion, which is done by calling the
corresponding hashtable method, `hashtable_find()`. It returns the counter.

`index_delete()` deletes the entire index. It utilizes helper functions which 
is passed into the corresponding delete methods for *hashtable* and *set* in
order to accomplish this.

`index_save()` takes a given filename and writes the entire index in the 
appropriate format (one word per line, followed by id and count) to it. It 
utilizes the corresponding hashtable iterate method and also helper functions.

`index_load()` takes a given filename and writes an index from it. It returns 
the index upon success, or exits with a non-zero return value if it 
enocounters an error. This method parses the file and utilizes 
`index_insert()` in order to create the index.

`index_page()` takes an opened file, its filename, and ID, and reads the HTML
before creating a dummy webpage to read words from it. For any word with a 
length greater than 2, it uses `add_word()` to insert it into the correct
counter.

`add_word()` takes a word and its ID and normalizes it before either adding it
to the counter if it already exists, or creates a new counter and adds it.
It then calls `index_insert()` to insert the word, counter pair to the index.

Errors are all printed to stderr, and exit with a non-zero exit code.











