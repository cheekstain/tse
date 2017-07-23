/* 
 * set.c - 'set' module
 * referenced bag.c
 *
 * see set.h for more information.
 *
 * Christina Lu, July 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
	void *item;                 // pointer to data for this item
	char *key;			  // key
	struct setnode *next;		  // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
	struct setnode *head; 			// head of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/**************** set_new() ****************/
set_t *set_new(void)
{
	set_t *set = count_malloc(sizeof(set_t));

	if (set == NULL) {
		return NULL; // error allocating set
	} else {
		// initialize contents of set 
		set->head = NULL;
		return set;
	}
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item) {
	if (set != NULL && key != NULL && item != NULL) {
		for (setnode_t *node = set->head; node != NULL;
			       			node = node->next) {
			// check for duplicate key;
			if (strcmp(node->key, key) == 0) {
				return false;
			}
		}
		// allocate a new node to add to list
		setnode_t *new = setnode_new(key, item);
		if (new != NULL) {
			// add it to the head of the list
			new->next = set->head;
			set->head = new;
			
			return true;
		}
	} 
	return false;
}

/**************** setnode_new() ****************/
/* Allocate and initialize a setnode */
static setnode_t *setnode_new(const char *key, void *item) 
{
	setnode_t *node = count_malloc(sizeof(setnode_t));

	char *key_copy = count_malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(key_copy, key);

	if (node != NULL) {
		node->item = item;
		node->key = key_copy;
		node->next = NULL;
		return node;
	} else {
		// error allocating memory
		return NULL;
	}
}

/**************** set_find() ****************/
void *set_find(set_t *set, const char *key)
{
	if (set == NULL) {
		return NULL;
	} else if (key == NULL) {
		return NULL;
	} else {
		for (setnode_t *node = set->head; node != NULL; 
						node = node->next) {
			if (strcmp(node->key, key) == 0) {
				return node->item;
			}
		}
		return NULL;
	}
	
}

/**************** set_print ****************/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) )
{
	if (fp != NULL) {
		if (set != NULL) {
			fputc('{', fp);
			if (itemprint != NULL) {
				for (setnode_t *node = set->head; node != NULL;
							node = node->next) {
				// print this node
					fputc(' ', fp);
					(*itemprint)(fp, node->key, node->item);
				}
			}
			fputc('}', fp);
		} else {
			fputs("(null)", fp);
		}
	}
}

/**************** set_iterate ****************/
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ) {
	if (set != NULL && itemfunc != NULL) {
		// call itemfunc with arg on each pair
		for (setnode_t *node = set->head; node != NULL; 
						node = node->next) {
			(*itemfunc)(arg, node->key, node->item);
		}
	}
}

/**************** set_delete ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) ) {
	if (set != NULL) {
		for (setnode_t *node = set->head; node != NULL; ) {
			if (itemdelete != NULL) {;
				(*itemdelete)(node->item);
			}
			setnode_t *next = node->next;
			count_free(node->key);
			count_free(node);
			node = next;
		}

		count_free(set);
	}
}

