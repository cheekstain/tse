/* 
 * indexer.c - CS50 Lab 5, part of TSE
 *
 * The TSE *indexer* is a standalone program that reads the document files
 * produced by the TSE crawler, builds an index, and writes that index to a 
 * file. 
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"
#include "index.h"
#include "pagedir.h"

/**************** global functions ****************/
/*
 * Index does the bulk of the indexer's work. It takes parameters 
 * page_directory and filename that have already been checked by main() and
 * initializes an index structure, builds it from the files within 
 * page_directory, and saves the index to the given filename, before cleaning
 * up at the end.
 */
void start_index(char* page_directory, char* filename);

/*
 * This method takes the page_directory and index, and builds the index from
 * the files within the given page_directory. 
 */
void index_build(char* page_directory, index_t *ht);

/**************** main() ********************/

int main(int argc, char* argv[])
{
	// check parameters
	// check number of arguments
	if (argc != 3) {
		fprintf(stderr, "usage: page_directory filename\n");
		exit(2);
	}

	// check directory
	char* page_directory = argv[1];
	if (!is_crawler_directory(page_directory)) {
		exit(3);
	}

	// check file
	char* filename = argv[2];
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "file is not writable\n");
		exit(4);
	} else {
		fclose(fp);
	}

	// calls the indexer
	start_index(page_directory, filename);
}

/*************** start_index() *****************/
void start_index(char* page_directory, char* filename)
{
	index_t *ht = index_new(800);

	index_build(page_directory, ht);
	index_save(ht, filename);
	
	//clean up
	index_delete(ht);
}

/************* index_build() ***************/
void index_build(char* page_directory, index_t *ht)
{
	// make file name
	char* filename;
	int id = 1;
	filename = make_filename(page_directory, id);
	if (filename == NULL) {
		count_free(filename);
		exit(1);
	}

	FILE *fp;
	fp = fopen(filename, "r");
	while (fp != NULL) {
		// update index with page
		index_page(ht, fp, filename, id);	
		
		id++;
		count_free(filename); 
		filename = make_filename(page_directory, id);
		if (filename == NULL) {
			count_free(filename);
			exit(1);
		}

		// attempt to open next file
		fp = fopen(filename, "r");
	}

	count_free(filename);
}











