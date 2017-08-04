/* 
 * counters.c - 'counters' module
 *
 * see counters.h for more information.
 *
 * Christina Lu, July 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counter {
	int *key;
	int *count;
	struct counter *next;
} counter_t;

/**************** global types ****************/
typedef struct counters {
	struct counter *head;
	struct counter *tail;
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counter_t *counter_new(int key);

/**************** counters_new() ****************/
counters_t *counters_new(void)
{
	counters_t *counters = count_malloc(sizeof(counters_t));

	if (counters == NULL) {
		return NULL; // error allocating counters
	} else {
		// initialize contents of counters
		counters->head = NULL;
		counters->tail = NULL;
		return counters;
	}
}

/**************** counters_add() ****************/
int counters_add(counters_t *ctrs, const int key)
{
	if (ctrs != NULL && key >= 0) {
		for (counter_t *counter = ctrs->head; counter != NULL;
						counter = counter->next) {
			if ((*counter->key) == key) { // check if counter exists
				// increment existing counter, return new value
				(*counter->count)++;	
				return (*counter->count);
			} 
		}
		// create a new counter, return value
		counter_t *new = counter_new(key);
		if (new != NULL) {
			if (ctrs->tail != NULL) {
				ctrs->tail->next = new;
			} else {
				ctrs->head = new;
			}
			ctrs->tail = new;
			*new->count = 1;
			return *new->count;
		}
		return 0;
	} else {
		return 0;
	}
}

/**************** counter_new() ****************/
/* Allocate and initialize a counter */
static counter_t *counter_new(int key)
{
	counter_t *counter = count_malloc(sizeof(counter_t));
	counter->count = malloc(sizeof(int));
	counter->key = malloc(sizeof(int));

	if (counter != NULL && counter->count != NULL && counter->key != NULL) {
		*counter->count = 1;
		*counter->key = key;
		counter->next = NULL;
		return counter;
	} else {
		// error allocating memory
		return NULL;
	}

}

/**************** counters_get() ****************/
int counters_get(counters_t *ctrs, const int key)
{
	if (ctrs != NULL) {
		for (counter_t *counter = ctrs->head; counter != NULL;
						counter = counter->next) {
			if (key == *counter->key) { // get value of counter
				return *counter->count;
			}
		}
		return 0;
	} else {
		return 0;
	}
}

/**************** counters_set() ****************/
void counters_set(counters_t *ctrs, const int key, int count)
{
	bool exist = false;
	if (ctrs != NULL && key >= 0 && count >= 0) {
		for (counter_t *counter = ctrs->head; counter != NULL;
						counter = counter->next) {
			if (key == *counter->key) { // change value of counter
				*counter->count = count;
				exist = true;
			}
		}
		// add new counter if key does not exist
		if (!exist) {
			counter_t *new = counter_new(key);
			if (new != NULL) {
				if (ctrs->tail != NULL) {
					ctrs->tail->next = new;
				} else {
					ctrs->head = new;
				}
				ctrs->tail = new;
				*new->count = 1;
				
			}
		}
	}
}

/**************** counters_print() ****************/
void counters_print(counters_t *ctrs, FILE *fp)
{
	if (fp != NULL) {
		if (ctrs != NULL) {
			fputc('{', fp);
				for (counter_t *counter = ctrs->head; 
						counter != NULL; 
						counter = counter->next) {
					if (counter->key == NULL || 
						       counter->count == NULL) {
						fprintf(fp, "[(null)]");
					} else {
						fprintf(fp, " [counter %d: %d]",
							*counter->key,
							*counter->count);
					}
				}
			fputs("}\n", fp);
		} else {
			fputs("(null)", fp);
		}
	}
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count))
{
	if (ctrs != NULL && itemfunc != NULL) {
		// call itemfunc on each counter
		for (counter_t *counter = ctrs->head; counter != NULL;
						counter = counter->next) {
			(*itemfunc)(arg, *counter->key, *counter->count);
		}
	}
}

/**************** counters_delete() ****************/
void counters_delete(counters_t *ctrs)
{
	if (ctrs != NULL) {
		for (counter_t *counter = ctrs->head; counter != NULL; ) {
			counter_t *next = counter->next;
			count_free(counter->key);
			count_free(counter->count);
			count_free(counter);
			counter = next;
		}
		count_free(ctrs);
	}
}

