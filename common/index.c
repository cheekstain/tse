/* 
 * index.c - TSE 'index' module
 *
 * see index.h for more information.
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "word.h"
#include "index.h"

/**************** global functions ****************/
/* see index.h for comments about exported functions */

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** local functions *************/
static void delete_helper(void *item);
static void print_word(void *arg, const char *key, void *ctrs);
static void print_count(void *arg, const int key, int count);

/**************** index_new() ****************/
index_t *index_new(const int num_slots)
{
	index_t *ht;

	ht = hashtable_new(num_slots);
	if (ht == NULL) {
		fprintf(stderr, "failed to allocate index");
		exit(1);
	}
	
	return ht;
}

/**************** index_insert() ****************/
bool index_insert(index_t *ht, char *key, counters_t *ctrs)
{
	NormalizeWord(key);
	return hashtable_insert(ht, key, ctrs);
}

/**************** index_find() ****************/
counters_t *index_find(index_t *ht, const char *key)
{
	return (counters_t*) hashtable_find(ht, key);
}

/**************** index_delete() ****************/
void index_delete(index_t *ht)
{
	hashtable_delete(ht, delete_helper);
}

static void delete_helper(void *item)
{
	if (item) {
		counters_delete((counters_t*) item);
	}
}
 
/*************** index_save() *****************/
void index_save(index_t *ht, char* filename)
{
	// create new file
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "error opening file %s\n", filename);
		exit(2);
	}

	// write to file
	hashtable_iterate(ht, fp, print_word);

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %s\n", filename);
		exit(2);
	}
}

/* print_word()
 * helper function to pass into hashtable_iterate to print each word
 */
static void print_word(void *arg, const char *key, void *ctrs)
{
	FILE *fp = arg;
		
	fprintf(fp, "%s", key);
	counters_iterate(ctrs, fp, print_count);
	fprintf(fp, "\n");
}

/* print_count()
 * helper function to pass into counters_iterate to print the counts
 */
static void print_count(void *arg, const int key, int count)
{
	FILE *fp = arg;

	fprintf(fp,  " %d %d", key, count);
}

/************** index_load() ***************/
void index_load(index_t* ht, char *filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file %s not found\n", filename);
		exit(2);
	}

	int size = lines_in_file(fp);
	ht = index_new(size);
	if (ht == NULL) {
		fprintf(stderr, "index_load index allocation failed\n");
		exit(1);
	}
		
	char *word;
	int id;
	int count;
	while((word = readwordp(fp)) != NULL) {
		counters_t *ctrs = counters_new();
		while (fscanf(fp, "%d %d ", &id, &count) == 2) {
			counters_set(ctrs, id, count);
		}
		index_insert(ht, word, ctrs);
	}

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %s\n", filename);
		exit(2);
	}
	
	count_free(word);
}

/************** index_page() ***************/
void index_page(index_t* ht, FILE *fp, char* filename, int id)
{
	// read html from file to string
        char *html;
        if ((html = readfilep(fp)) == NULL) {
                fprintf(stderr, "error reading file %s\n", filename);
		exit(4);
        }
	
	if (fclose(fp) != 0) {
                fprintf(stderr, "error closing file %s\n", filename);
                exit(4);
        }

	// create dummy webpage 
	char *url = "/";
	webpage_t *page = webpage_new(url, 0, html);

	// read words
	int pos = 0;
	char *result;
	while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
		if (result != NULL && strlen(result) > 2) {
			add_word(ht, result, id);
		}
		count_free(result);
	}

	// clean up
	webpage_delete(page);
 }

/********************* add_word() ******************/
void add_word(index_t *ht, char *word, int id) {
        NormalizeWord(word);
        counters_t *ctrs = index_find(ht, word);

        if (ctrs != NULL) {
                // word already exists, increment counter
                counters_add(ctrs, id);
        } else {
                // add new word to table, create new counter
                ctrs = counters_new();
                if (ctrs == NULL) {
                        fprintf(stderr, "failed to allocate counter\n");
                        exit(1);
                }

                counters_add(ctrs, id);
                index_insert(ht, word, ctrs);
        }
}
