# CS50 Lab 4
## Christina Lu, CS50 Summer 2017

### crawler

The *crawler* crawls a website and retrieves webpages starting with a 
specified URL. It parses the initial webpage, extracts any embedded URLs and
retrieves those pages, and crawls the pages found at those URLs, but limits
itself to `maxDepth` hops from the seed URL and to URLs that are ‘internal’
to the designated CS50 server. When the crawler process is complete, the 
indexing of the collected documents can begin.

### Usage

The *crawler*, defined in `crawler.h` and implemented in `crawler.c`, 
implements a crawler and includes the following functions:

```
c
/* Crawls through webpages from a seed URL, extracts and searches embedded
 * URLs up to the max_depth. 
 */
bool crawl(char* seed_url, char* page_directory, int max_depth);

/* Explores a webpage to find links. Checks URLs before adding where 
 * appropriate. Creates new webpages for found URLs.
 */
void explore_webpage(webpage_t* page, bag_t* unexplored_pages, 
                                hashtable_t* seen_urls);

/* Write page HTML to file, includes depth and URL.
 */
bool page_saver(webpage_t* page, char* page_directory, int id);
```

### Implementation

See [IMPLEMENTATION](IMPLEMENTATION.md) for details about implementation.

### Assumptions

Input URLs and directories must both end with a `\`. For pages with many 
large amounts of links and/or depth, program will run significantly slower.
Thus, will not accept depth greater than 10.

### Compilation

To compile, simply `make`.

To test, simply `make test` or to unit test, `make unit`.
See [TESTING](TESTING.md) for details of testing.

