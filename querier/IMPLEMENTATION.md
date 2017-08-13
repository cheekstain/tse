
# CS50 Lab 6
## Christina Lu, CS50 Summer 2017

### Querier Implementation

The methods within *querier* can be organized by the tasks it must do.
The `main()` function takes the arguments, initializes and loads the 
`index` data structure that contains the pertinent word frequency 
information, and calls the method to process the query. At the end, it cleans,
up by deleting the index.

`check_parameters()` is called by `main()`, and it does just that. It makes
sure that there are exactly 3 inputs, that the given directory is a 
crawler directory, and that the given filename is readable. It returns true
upon success, false otherwise.

### Query Parsing

`process_query()` is the framework for the majority of query legworkd. It 
takes input from stdin, calls a function to clean the input, then calls 
the functions to print, validate, and run the query in turn. It keeps
reading from stdin until an EOF is reached, or there is an error.

`fetch_words()` is given a line of input from `process_query()` and it finds
the words within it and returns them as a `char**` array. It accomplishes this
by using pointers to slide along the line on input, looking for alpha
characters and then spaces, so it knows to end that word with NULL. It also 
checks for non-alpha/space characters, which will prompt and error message.
It returns an `int` count, the number of words it has found, which is useful 
as well. All words that it adds to the array are normalized.

`print_query()` prints the cleaned up query to stdout, with a little syntax
before it.

`check_query()` further validates the query. It returns true or false, and 
makes sure that *and*/*or* cannot be the first or last words, or next
to another literal. 

`is_literal()` is a helper function utilized when checking query and also
while searching. It simple returns true if the `char*` input is *and*/*or*
and false otherwise.

### Query Searching

Now that the query has been validated, `run_query()` provides the bulk of the
framework for searching for documents that match, as well as calculating the
scores of each file at the same time. It creates a new `index` data 
structure to keep track of the *and sequences*, initializes a `counter` to 
keep track of the final scores, and calls the methods which find the
scores, aggregate them, and prints them, before deletings its structures
and cleaning up after itself.

The other method that does a lot of legwork for searching is `get_scores()`
which looks at each word, stores the `counter` which represent its file
occurances, and finds the union or disjunction of them according to the
query. It utilizes many helper functions, including those that iterate
over the counters.

Two such methods are `counters_intersect()` and `counter_intersect_helper()`.
When called, they find the intersection of two counters and set the value
of one counter to the appropriate score. 

Other useful helper methods are `aggregate_scores()`, `sum_scores()` and
`duplicate_counter()`. The first method loops through each counter within
the `index` which represents the *and sequence*, and the second finds the
sum of all the *and sequences* to find the total. `duplicate_counter()` does
what it says, when passed into `counters_iterate()` with the new copy as an
`arg*`.

### Results: Sorting, Ranking, and Reporting

`sort_pages()` takes the `counter` of final accumulated scores and makes 
sense of them by making them into the `all_docs` struc and calling sort
on them. A helper function it uses is `counters_count()` which loops through
the structure and finds the total number of counters.

`counters_sort()` does the bulk of the sorting work. It is passed through
`counters_iterate()` as well, and uses insertion sort to create an array of
`doc_t*` structures that are arranged in decreasing order. 

Finally, `print_results()` takes the `all_docs` struct and prints them in 
the appropriate fashion. It reports the documents matched, if any, along with
their scores, IDs, and URLs. Thus, the query is complete.





                                                             

