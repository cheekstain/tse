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
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../libcs50/memory.h"
#include "crawler.h"


/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see crawler.h for comments about exported functions */

/**************** local functions ****************/
static void hashdelete(void *item); 

/**************** main() ****************/

int main(int argc, const char* argv[])
{
	// check number of arguments
	if (argc != 4) {
		fprintf(stderr, "usage: seed_url page_directory max_depth\n");
		exit(1);
	} 

	// check seed url
	char* seed_url = count_malloc(sizeof(char) * (strlen(argv[1])+1));
	strcpy(seed_url, argv[1]);
	if (!IsInternalURL(seed_url)) {
		fprintf(stderr, "not a valid seed url\n");
		exit(2);
	}
	
	char str[80];
	char* page_directory = count_malloc(sizeof(char) * (strlen(argv[1])+1));
	strcpy(page_directory, argv[2]);
	strcpy(str, argv[2]);
	strcat(str, "/.crawler");

	FILE *fp = fopen(str, "w");
	if (fp == NULL) {
		fprintf (stderr, "directory does not exist or is not writable\n");
		exit(3);
	} else {
		fclose(fp);
	}
	count_free(str);

	int max_depth;
	if (sscanf(argv[3], "%d", &max_depth) == 0) {
		fprintf(stderr, "max depth must be an int greater than 0\n");
		exit(4);
	}

	if (!crawl(seed_url, page_directory, max_depth)) {
		exit(5);
	}

	count_free(seed_url);
	count_free(page_directory);
}

/**************** crawler() ****************/
bool crawl(char* seed_url, char* page_directory, int max_depth)
{
	// initialize bag & hashtable
	bag_t* unexplored_pages = bag_new();
	hashtable_t* seen_urls = hashtable_new(20);
	int current_depth = 0;

	if (unexplored_pages == NULL || seen_urls == NULL) {
		fprintf(stderr, "error allocating memory\n");
		return false;
	}
	
	// initilize seed page
	char* dummy = "a";
	webpage_t* seed_page = webpage_new(seed_url, current_depth, NULL);
	bag_insert(unexplored_pages, seed_page);
	hashtable_insert(seen_urls, seed_url, dummy);

	int id = 1;
	// crawl while bag is empth
	webpage_t *current_page;
	while (current_page != NULL) {
		if (!webpage_fetch(current_page)) {
			fprintf(stderr, "error with html\n");
			return false;
		}
		
		page_saver(current_page, page_directory, id);
		
		int depth = webpage_getDepth(current_page);
		if (depth < max_depth) {
			explore_webpage(current_page, unexplored_pages, 
					seen_urls);
		}
		current_page = bag_extract(unexplored_pages);
	}

	// clean up at the end
	bag_delete(unexplored_pages, webpage_delete);
	hashtable_delete(seen_urls, hashdelete);

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

/* delete each item in the hash table
 * code modified from bagtest.c
 */
 static void hashdelete(void *item)
 {
     if (item) {
            count_free(item);
    }
 }


void page_saver(webpage_t* page, char* page_directory, int id) 
{
	// create file name
	char str[120];
	char num[100];
	sprintf(num, "%d", id);
	strcpy(str, page_directory);
	strcat(str, "/");
	strcat(str, num);
	
	// create new file
	FILE *fp = fopen(str, "w");
	if (fp == NULL) {
    	fprintf(stderr, "error opening file %d\n", id);
	}

	// write to file
	char* url = webpage_getURL(page);
	if (url == NULL) {
		fprintf(stderr, "error writing url for file %d\n", id);
	} else {
		fprintf(fp, "%s\n", url);
		count_free(url);
	}
	
	int depth = webpage_getDepth(page);
	if (depth < 0) {
		fprintf(stderr, "error writing depth for file %d\n", id);
	} else {
		fprintf(fp, "%d\n", depth);
	}

	char* html = webpage_getHTML(page);
	if (html == NULL) {
		fprintf(stderr, "error writing html for file %d\n", id);
	} else {
		fprintf(fp, "%s", html);
		count_free(html);
	}
	
	
	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %d\n", id);
	}

	count_free(str);
	count_free(num);
}




















