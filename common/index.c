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
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "word.h"
#include "index.h"

/**************** global functions ****************/
/* see index.h for comments about exported functions */

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** local functions *************/
static void print_word(void *arg, const char *key, void *item);
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
bool index_insert(index_t *ht, const char *key, counters_t *ctrs)
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
	hashtable_delete(ht, counters_delete);
}
 
/*************** index_save() *****************/
bool index_save(index_t *ht, char* filename)
{
	// create new file
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "error opening file %s\n", filename);
		return false;
	}

	// write to file
	hashtable_iterate(ht, fp, print_word);

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %s\n", filename);
		return false;
	}
	
	return true;
}

/* print_word()
 * helper function to pass into hashtable_iterate to print each word
 */
static void print_word(void *arg, const char *key, counters_t ctrs)
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
index_t *index_load(char* filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	
	index_t *ht = index_new(500);
	if (ht == NULL) {
		fprintf(stderr, "index_load allocation failed");
		return NULL;
	}
		
	char *word;
	int id;
	int count;
	while((word = readwordp(fp)) != NULL) {
		counters_t *ctrs = counters_new();
		while (fscanf(fp, "%d %d", &id, &count) == 2) {
			counters_set(ctrs, id, count);
		}
		index_insert(ht, word, ctrs);
	}

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %s\n", filename);
		return NULL;
	}
	
	count_free(word);
	// free others?
	
	return ht;
}
