/*
 * crawler.h - header file for crawler
 * 
 * The TSE crawler is a standalone program that crawls the web and retrieves
 * webpages starting from a “seed” URL. It parses the seed webpage, extracts 
 * any embedded URLs, then retrieves each of those pages, recursively, but 
 * limiting its exploration to a given “depth”.
 * 
 * Christina Lu, July 2017
 */

#ifndef __CRAWLER_H
#define __CRAWLER_H



/**************** functions ****************/

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
void page_saver(webpage_t* page, char* page_directory, int id);

#endif // __CRAWLER_H
