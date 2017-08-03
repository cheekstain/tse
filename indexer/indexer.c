/* 
 * indexer.c - CS50 Lab 5, part of TSE
 *
 * see indexer.h for more information.
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/memory.h"
#include "../common/index.h"
#include "../common/pagedir.h"
#include "indexer.h"

/**************** global functions ****************/
/* see indexer.h for comments about exported functions */

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
	if (!is_crawler_directory(argv[1])) {
		exit(3);
	}

	// check file
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "file is not writable\n");
		exit(4);
	} else {
		fclose(fp);
	}

	// calls the indexer
	index(page_directory, filename);
}

/*************** index() *****************/
void index(char* page_directory, char* filename)
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
		exit(1);
	}

	FILE *fp;
	fp = fopen(filename, "r");
	while (fp != NULL) {
		// update index with page
		index_page(ht, fp, filename, id);

		// prepare for next file
		if (fclose(fp) != 0) {
			fprintf(stderr, "error closing file %s\n", filename);
			exit(4);
		}

		count_free(filename);
		id++;
		filename = make_filename(page_directory, id);
		if (filename == NULL) {
			exit(1);
		}

		// attempt to open next file
		fp = fopen(filename, "r");
	}

	count_free(filename);
}











