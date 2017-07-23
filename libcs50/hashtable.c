/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Christina Lu, July 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "set.h"
#include "jhash.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
	struct set **array;
	int slots;
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* none */

/**************** hashtable_new() ****************/
hashtable_t *hashtable_new(const int num_slots)
{
	hashtable_t *ht = count_malloc(sizeof(hashtable_t));
	ht->slots = num_slots;
	ht->array = count_calloc(num_slots, sizeof(struct set*));

	// initialize array sets
	for (int i = 0; i < num_slots; i++) {
		ht->array[i] = set_new();
		if (ht->array[i] == NULL) {
			return NULL; // error allocating
		}
	} 

	if (ht == NULL || ht->array == NULL) {
		return NULL; // error allocating 
	} else {
		return ht;
	}
}

/**************** hashtable_insert() ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
	if (ht != NULL && key != NULL && item != NULL) {
		int hash = JenkinsHash(key, ht->slots);	
		if (set_find(ht->array[hash], key) == NULL) {
			// check if key already exists
			if (set_insert(ht->array[hash], key, item)) {
				return true; // inserted properly
			} 
		} 		
	} 
	return false;
}

/**************** hashtable_find() ****************/
void *hashtable_find(hashtable_t *ht, const char *key)
{
	if (ht == NULL) {
		return NULL;
	} else if (key == NULL) {
		return NULL;
	} else {
		int hash = JenkinsHash(key, ht->slots);
		return set_find(ht->array[hash], key);
	}
}

/**************** hashtable_print() ****************/
void hashtable_print(hashtable_t *ht, FILE *fp,
                void (*itemprint)(FILE *fp, const char *key, void *item) )
{
	if (fp != NULL) {
		if (ht != NULL) {
			fputc('{', fp);
				if (itemprint != NULL) {
					for (int i = 0; i < ht->slots; i++) {
						fprintf(fp, "[hash %d: ", i);
						set_print(ht->array[i], fp,
							       	itemprint);
						fputc(']', fp);
					}
				}
			fputs("}\n", fp);
		} else {
			fputs("(null)", fp);
		}
	}
}

/**************** hashtable_iterate() ****************/
void hashtable_iterate(hashtable_t *ht, void *arg,
            	void (*itemfunc)(void *arg, const char *key, void *item) )
{
	if (ht != NULL && itemfunc != NULL) {
		for (int i = 0; i < ht->slots; i++) {
			set_iterate(ht->array[i], arg, itemfunc);
		}
	}
}


/**************** hashtable_delete() ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) )
{
	if (ht != NULL) {
		for (int i = 0; i < ht->slots; i++) {
			set_delete(ht->array[i], itemdelete);
		}
		count_free(ht->array);
		count_free(ht);
	}
}






