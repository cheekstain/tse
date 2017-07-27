# CS50 Lab 4
## Christina Lu, CS50 Summer 2017

### Crawler Implementation

We implement the crawler as a series of modules: `main()`, `crawl()`, 
`explore_webpage()`, and `page_saver()`. It uses three data structures: 
a *bag* of page (URL, depth) structures, a *set* of URLs (indirectly used by 
hashtable), and a *hashtable* of URLs. The crawler also makes use of the
webpage module to deal with webpages.

`main()` parses the arguments and calls `crawl()`. It checks to make sure that
arguments are correct and valid, returning errors if needed. Then, it 
initializes the variables necessary for `crawl()`: `seed_url`,
`page_directory`, and `max_depth` and calls it. At the end, it cleans up and
frees those variables.

`crawl()` is the main implentation of the crawler. It begins by intializing
the bag and hashtable, as well as setting the current depth to 0. It returns
FALSE if it runs into any errors.

It initializes the seed page, putting it into the bag and the hashtable, then
initializes the count for IDs.

While the bag is empty, the `crawl()` takes on page at a time from it and 
fetches the HTML, calls `page_saver()` to save the page to an external file, 
and calls `explore_webpage()` to explore the webpage for more links if its 
depth is less than the max depth. After the page has been explored, it is
deleted and the current page updated to a new one, freshly extracted from the
bag.

At the end, `crawl()` cleans up by deleting the bag and the hashtable and
returning true if everything was sucessful.

The `explore_webpage()` module takes a page and scans it for links to other
webpages. If the URl is internal and valid, it is inserted to the hashtable.
If it is inserted successfully, that means the URL has not been visited 
before, and it is then inserted into the bag to be searched later.

The `page_saver()` module takes a page and writes the necessary information
to an external file in a given directory. Files for each webpage are named
by integer, starting from 1 and incrementing. File contents include the URL,
depth, and unmodified HTML.

If there are any errors in printing to the file, a message is written to 
stderr but the program continues. If there are other errors, the module 
returns false. The module returns true iff writing the file to the correct
directory is sucessful.


