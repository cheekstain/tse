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
	if (fp == NULL) {
		fprintf(stderr, "file %s not found\n", filename);
		return NULL;
	}

	int size = lines_in_file(fp);
	index_t *ht = index_new(size);
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
		return NULL;
	}
	
	count_free(word);
	
	return ht;
}

/************** index_page() ***************/
index_t *index_page(char* filename)
{
        FILE *fp;
        fp = fopen(filename, "r");
        if (fp == NULL) {
                fprintf(stderr, "file %s not found\n", filename);
                return NULL;
        }

        index_t *ht = index_new(800);
        if (ht == NULL) {
                fprintf(stderr, "index_page index allocation failure");
                return NULL;
        }

        char *str;
        while ((str = readwordp(fp)) != NULL) {
                int c;
                char word[100];
                char *wp = word;

                for (int i = 0; i < strlen(str); i++) {
                        c = string[i];
                        if (isalpha(c)) {
                                if (strlen(word) == 0) {
                                        wp += sprintf(word, "%c", c);
                                } else {
                                        wp += sprintf(wp, "%c", c);
                                }
                        } else {
                                if (strlen(word) > 0) {
                                        add_word(ht, word, id);
                                        memset(word, '\0', 100);
                                        wp = word;
                                }
                        }
                        if (strlen(word) > 0) {
                                add_word(ht, word, id);

                        }
                }
                count_free(wp);
        }

        if (fclose(fp) != 0) {
                fprintf(stderr, "error closing file %s\n", filename);
                return NULL;
        }

        count_free(str);

        return ht;
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
