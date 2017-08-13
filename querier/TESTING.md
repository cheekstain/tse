
# CS50 Lab 6
## Christina Lu, CS50 Summer 2017

### Querier Testing

### Unit Testing

In the beginning, unit testing was used to test the functionality of the
*querier*. Three unit tests were performed, to test the early capabilites:

* The first tests `fetch_words()` and gives it input of different 
capitalization and spacing in order to ensure that it does that proper thing.
Since there were early bugs associated with sliding the pointers, it was
very useful.
* The next tests `check_query()` and `print_query()` by giving several 
different invalid queries, such as where the *and*/*or* operators were on the
sides or next to each other. 
* `get_scores()` was tested next, which took just one *and* statement along
with a sample file `./index3` from 

These tests are limited in size and parts of them were hard-coded, but they 
were very important in testing early functionality and dealing with bugs.

The unit test can be ran from the [Makefile](Makefile) by `make unit`.

### Further Testing

It became difficult to perform structured tests after more compoments were
added to the *querier* so most of the following tests were done in an
interactive environment.

Test files give by the Professor were used to spotcheck query results, and
such tests were done until the end. Edge cases were checked and smoothed 
out. Both Valgrind and GDB were used to pinpoint bugs in the code.

                                                             

