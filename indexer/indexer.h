/* 
 * indexer.h - header file for TSE indexer
 *
 * The TSE *indexer* is a standalone program that reads the document files 
 * produced by the TSE crawler, builds an index, and writes that index to a
 * file. 
 *
 * Christina Lu, August 2017
 */

#ifndef __INDEXER_H
#define __INDEXER_H


/******************** functions *******************/

/*
 * Index does the bulk of the indexer's work. It takes parameters 
 * page_directory and filename that have already been checked by main() and
 * initializes an index structure, builds it from the files within 
 * page_directory, and saves the index to the given filename, before cleaning
 * up at the end.
 */
void index(char* page_directory, char* filename);

/*
 * This method takes the page_directory and index, and builds the index from
 * the files within the given page_directory. 
 */
void index_build(char* page_directory, index_t *ht);

#endif // __INDEXER_H
