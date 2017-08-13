
# CS50 Lab 6
## Christina Lu, CS50 Summer 2017

### Querier Design

The *querier's* tasks include:

* checking parameters
* cleaning and parsing user queries from stdin
* identifying the documents that fulfill the query
* scoring and ranking them in decreasing order
* printing the search result to stdout in an organized fashion

As such, tasks are organized into methods and streamlined within several 
functions that provide the framework. More generally, there are functions
that deal with parsing query, fulfilling query, scoring/ranking, and printing,
as well as several helper functions.

### Data Structures

The *querier* uses several data structures, especially the `counters` and
`index` modules, to organize information.

The `two_counter` structure is simply enabling two counters to be passed as
one argument, for convenience. It is especially useful when trying to find
the union and disjunctions. It contains two `counters_t*` pointers, one to
each counters structure.

The `doc` structure is used to keep track of two `int` variables, the key 
ID of a file and an integer counter. The `count` variable can be used to
either keep track of the count of word occurances, or the score of the file.
`doc` allows these two variables to be bundled together and placed into an
array for easy access.

The `all_docs` structure is used to contain a `doc_t**` array as well as an
`int` for the number of elements. This allows simple iteration and is
helpful when performing sorting.

The `index` structure is also used, but this time as a structure that contains
all the *and sequences* of a particular query. Since it keeps track of 
(word, counters) pairings, it is especially useful when trying to find the 
union and disjunctions of counters. Each counters struct within this 
particular `index` structure contains filed IDs paired off with the score
of that file, as accumulated through the *and sequence*. This allows all 
counters within `index` to be compared to one another to find their sums.
The `char*` key is completely arbitrary, as the purpose of the structure
is mostly to keep all the *and sequnece* scores in a handy place.

