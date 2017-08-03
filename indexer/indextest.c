/* 
 * indextest.c - CS50 Lab 5, part of TSE
 *
 * The *index tester* is the companion of the indexer; it loads an index file 
 * produced by the indexer and saves it to another file.
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"

/**************** global functions ****************/

/**************** main() ********************/

int main(int argc, char* argv[])
{
	// check number of arguments
	if (argc != 3) {
		fprintf(stderr, "usage: page_directory filename\n");
		exit(2);
	}

	char* old_file = argv[1];
	char* new_file = argv[2];
	index_t *ht = index_load(old_file);
	index_save(ht, new_file);
	
	// clean up
	index_delete(ht);
	exit(0);
}


