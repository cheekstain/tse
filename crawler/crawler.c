/*
 * crawler.c - CS50 Lab 4, part of TSE
 * 
 * see crawler.h for more information.
 * 
 * Christina Lu, July 2017
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"
#include "crawler.h"

/**************** global functions ****************/
/* that is, visible outside this file */
/* see crawler.h for comments about exported functions */

/**************** main() ****************/

#ifndef UNIT_TEST
int main(int argc, const char* argv[])
{
	// check number of arguments
	if (argc != 4) {
		fprintf(stderr, "usage: seed_url page_directory max_depth\n");
		exit(1);
	} 

	// check seed url
	char* seed_url = count_malloc(sizeof(char) * (strlen(argv[1])+1));
	if (seed_url == NULL) {
		fprintf(stderr, "seed_url memory allocation error\n");
		exit(6);
	}
	strcpy(seed_url, argv[1]);
	if (!IsInternalURL(seed_url)) {
		fprintf(stderr, "not a valid seed url\n");
		exit(2);
	}
	
	// check directory
	char* str = count_malloc(sizeof(char) * (strlen(argv[2])+10));
	char* page_directory = count_malloc(sizeof(char) * (strlen(argv[2])+1));
	if (str == NULL || page_directory == NULL) {
		fprintf(stderr, "page_directory memory allocation error\n");
		exit(6);
	}
	strcpy(page_directory, argv[2]);
	strcpy(str, argv[2]);
	strcat(str, "/.crawler");

	FILE *fp = fopen(str, "w");
	if (fp == NULL) {
		fprintf (stderr, "dir does not exist or is not writable\n");
		exit(3);
	} else {
		fclose(fp);
	}
	count_free(str);

	// check max depth
	int max_depth;
	if ((sscanf(argv[3], "%d", &max_depth) == 0) || max_depth < 0 || max_depth > 10) {
		fprintf(stderr, "max depth must be an int greater than 0 and less than 10\n");
		exit(4);
	}
	
	// calls the crawler
	if (!crawl(seed_url, page_directory, max_depth)) {
		exit(5);
	}

	count_free(seed_url);
	count_free(page_directory);
}
#endif // UNIT_TEST

/**************** crawl() ****************/
bool crawl(char* seed_url, char* page_directory, int max_depth)
{
	// initialize bag & hashtable
	bag_t* unexplored_pages = bag_new();
	hashtable_t* seen_urls = hashtable_new(20);
	if (unexplored_pages == NULL || seen_urls == NULL) {
		fprintf(stderr, "error allocating bag/hashtable\n");
		return false;
	}
	
	int current_depth = 0;
	
	// initilize seed page
	char* dummy = "a";
	webpage_t* seed_page = webpage_new(seed_url, current_depth, NULL);
	if (seed_page == NULL) {
		fprintf(stderr, "error allocating seed webpage\n");
		return false;
	}
	bag_insert(unexplored_pages, seed_page);
	hashtable_insert(seen_urls, seed_url, dummy);

	int id = 1;

	// crawl through pages while bag is empty
	webpage_t *current_page = bag_extract(unexplored_pages);
	while (current_page != NULL) {
		if (!webpage_fetch(current_page)){
			char* name = webpage_getURL(current_page);
			fprintf(stderr, "%s does not exist\n", name);
		} else {	
			
			if (!page_saver(current_page, page_directory, id)) {
				return false;
			}

			id++;

			int depth = webpage_getDepth(current_page);
			if (depth < max_depth) {
				explore_webpage(current_page, unexplored_pages, 
						seen_urls);
			}
		}
		webpage_delete(current_page);
		current_page = bag_extract(unexplored_pages);
		
	}

	// clean up at the end
	bag_delete(unexplored_pages, webpage_delete);
	hashtable_delete(seen_urls, NULL);
	

	return true;
}

void explore_webpage(webpage_t* page, bag_t* unexplored_pages, 
				hashtable_t* seen_urls)
{
	int pos = 0;
	char *result;
	int depth = webpage_getDepth(page) + 1;

	char* dummy = "a";
	while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
    	if (result != NULL) {
    		if (IsInternalURL(result)) {
    			if(hashtable_insert(seen_urls, result, dummy)) {
    				webpage_t* new_page = webpage_new(result, depth,
					       	NULL);
    				bag_insert(unexplored_pages, new_page);	
			}
    		}
    		free(result);
    	}    
    }
}




/********************************************************
 **************** unit testing **************************
 ********************************************************/

#ifdef UNIT_TEST
#include "unittest.h"

/////////////////////////////////////
// attempt to save a webpage
int test_pagesaver() 
{
	START_TEST_CASE("pagesaver");

	// set up webpage
	char *url = "http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/";
	EXPECT(url != NULL);
	webpage_t *page = webpage_new(url, 0, NULL);
	EXPECT(page != NULL);
	
	// check that html is fetched
	EXPECT(webpage_fetch(page));

	// print webpage to file
	EXPECT(page_saver(page, "./unit_data/", 1));

	// clean up
	webpage_delete(page);	

	EXPECT(count_net() == 0);

	END_TEST_CASE;
  	return TEST_RESULT;
}

// helper function for test_explorewebpage()
// prints item
void simpleprint(FILE *arg, void*item)
{
	FILE *fp = arg;
	webpage_t *page = item;
	char *url = webpage_getURL(page);
	fprintf(fp, "%s ", url);
}


/////////////////////////////////////
// attempt to explore a webpage. prints contents of 
// bag of unexplored urls found on seed page.
	
int test_explorewebpage()
{
	START_TEST_CASE("explorewebpage");

	// set up webpage
	char *url = "http://old-www.cs.dartmouth.edu/~cs50/index.html";
	char *dummy = "a";
	EXPECT(url != NULL);
	webpage_t *page = webpage_new(url, 0, NULL);
	EXPECT(page != NULL);

	// check that html is fetched
	EXPECT(webpage_fetch(page));
	
	// set up bag and hashtable
	bag_t *unexplored = bag_new();
	hashtable_t *seen = hashtable_new(20);
	EXPECT(unexplored != NULL && seen != NULL);

	// add seed to unexplored bag and hashtable
	bag_insert(unexplored, page);
	EXPECT(hashtable_insert(seen, url, dummy));

	// print bag
	explore_webpage(page, unexplored, seen);
	printf("printing unexplored bag:\n");
	bag_print(unexplored, stdout, simpleprint);
	printf("\n\n");
	
	//clean up
	bag_delete(unexplored, webpage_delete);
	hashtable_delete(seen, NULL);

	EXPECT(count_net() == 0);

	END_TEST_CASE;
 	return TEST_RESULT;
}


////////////////////////////////////////////
// main()
int main(const int argc, const char *argv[]) 
{
	int failed = 0;

	failed += test_pagesaver();
	failed += test_explorewebpage();

	if (failed) {
   		printf("FAIL %d test cases\n", failed);
    		return failed;
  	} else {
    		printf("PASS all test cases\n");
    		return 0;
  	}
}


#endif // UNIT_TEST

