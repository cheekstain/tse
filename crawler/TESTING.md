# CS50 Lab 4
## Christina Lu, CS50 Summer 2017

### Crawler Testing

#### Unit Testing 

The crawler modules were first tested through unit testing, marked by the
flag `UNIT_TEST`. It tests the `page_saver()` and `explore_webpage()` modules.
It uses `unittest.h` copied from the one provided in the lecture.

It first tests `page_saver()` with `test_pagesaver()`. It sets up an test
webpage, fetches its HTML, and extracts information to a file. It then deletes
the webpage. The file is written to the directory `crawler/unit_data/`.

Then it tests `explore_webpage()` using `text_explorewebpage()`. It again
sets up a test webpage, fetches its HTML, then sets up a bag and a hashtable.
It explores the webpage for more links, then prints the unexplored bag at the
end to confirm that all valid links have been collected. It then cleans up.

The `main()` function counts the number of failed modules and prints to show
either success or failure. For failures, the test gives the line number.
`UNIT_TEST` also counts the net memory blocks used and confirms that it is 0.

The unit test can be called with `make unit`.

#### Further Testing

The crawler was tested further from the command line.

The crawler was first tested with invalid command line arguments: wrong
number of arguments, nonexistent directories, invalid integers, etc. Then it
was tested for invalid `seed_url`: non-existent server, non-internal server,
and valid server but non-existent page.

The crawler was then tested on a [playground created for it](http://old-www.cs.dartmouth.edu/~cheekstain/tse/).
It includes cross-linked webpages. Some are mentioned multiple times across
pages and across the set of pages. There are loops along with dead ends 
(including a nonexistent page that gets skipped over), and external links.
The files were written to the directory `crawler/test_data/` and 
cross-referenced each time for accuracy. Depths of: 0, 1, 2, 3, 4, 5 were
checked, as well as different seed URLs.

The crawler was then tested on the [wikipedia playground](http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/).
Depths of 0, 1, 2 were tested and cross-referenced for accuracy as well.

A selection of these tests can be called with `make test` which runs the 
script `test.sh`.

### Limitations

The processing power and speed significantly slows down as the depth
increases, as demonstrated by the *Wikipedia* example. Larger integers over 6
were not checked, integers over 10 are not accepted by the program.


