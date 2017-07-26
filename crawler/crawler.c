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
#include <webpage.h>
#include <hashtable.h>
#include <bag.h>
#include <crawler.h>

/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see crawler.h for comments about exported functions */
void crawl(char* seed_url, char* page_directory, int max_depth);
void explore_webpage(webpage_t* page, bag_t* unexplored_pages, 
				hashtable_t* seen_urls);

/**************** main() ****************/

int main(int argc, const char* argv[])
{
	// check number of arguments
	if (argc != 4) {
		fprintf(stderr, "usage: seed_url page_directory max_depth\n");
		exit(1);
	} 

	// check seed url
	char* seed_url = argv[1]
	if (!IsInternalURL(seed_url)) {
		fprintf(stderr, "not a valid seed url\n");
		exit(2);
	}
	
	char str[80];
	strcpy(str, argv[2]);
	strcat(str, "/.crawler");
	if (fopen(str, "w") == NULL) {
		fprintf (stderr, "directory does not exist or is not writable\n");
		exit(3);
	} else {
		fclose(str);
		char* page_directory = argv[2];
	}
	free(str);

	int max_depth;
	if (sscanf(argv[3], "%d", &max_depth) == 0) {
		fprintf(stderr, "max depth must be an integer greater than 0\n");
		exit(4);
	}

	if (!crawl(seed_url, page_directory, max_depth)) {
		exit(5);
	}
}

/**************** crawler() ****************/
bool crawl(char* seed_url, char* page_directory, int max_depth)
{
	// initialize bag & hashtable
	bag_t* unexplored_pages = bag_new();
	hashtable_t* seen_urls = hashtable_new();
	int current_depth = 0;

	if (unexplored_pages == NULL || seen_urls == NULL) {
		fprintf(stderr, "error allocating memory\n");
		return false;
	}
	
	// initilize seed page
	const char* dummy = "a";
	webpage_t* seed_page = webpage_new(seed_url, current_depth, NULL);
	bag_insert(unexplored_pages, seed_page);
	hashtable_insert(seen_urls, seed_url, dummy);

	// crawl while bag is empth
	while ((current_page = bag_extract(unexplored_pages)) != NULL) {
		if (!webpage_fetch(current_page)) {
			fprintf(stderr, "error with html\n");
			return false;
		}
		page_saver(current_page, page_directory);
		
		if ((int depth = webpage_getDepth(current_page)) < max_depth) {
			explore_webpage(current_page, unexplored_pages, seen_urls);
		}
	}

	
}

void explore_webpage(webpage_t* page, bag_t* unexplored_pages, 
				hashtable_t* seen_urls)
{
	int pos = 0;
	char *result;
	int depth = webpage_getDepth(page) + 1;

	const char* dummy = "a";
	while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
    	if (result != NULL) {
    		if (IsInternalURL(result)) {
    			if(hashtable_insert(seen_urls, result, dummy)) {
    				webpage_t* new_page = webpage_new(result, depth, NULL);
    				bag_insert(unexplored_pages, new_page);
    			}
    		}
    		free(result);
    	}    
    }
}

void page_saver(webpage_t* page, char* page_directory) 
{
	
}




















