/* 
 * querier.c - TSE 'querier' module
 *
 * The *querier* reads the index produced by the Indexer and the page files
 * produced by the Crawler, to interactively answer written queries entered by 
 * the user. 
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "counters.h"
#include "memory.h"
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct two_counters {
    counters_t *result;
    counters_t *ctrs;
} two_counters_t;

typedef struct doc {
	int key;
	int count;
} doc_t;

typedef struct all_docs {
	doc_t** pages;
	int size;
} all_docs_t;

/**************** functions ****************/
bool check_parameters(int argc, char* argv[]);

void process_query(char* page_directory, index_t* ht);
int fetch_words(char* line, char** words);
void print_query(char** words, int count);
bool check_query(char** words, int count);
void run_query(index_t *ht, char** words, int count, char* page_directory);

bool is_literal(char* word);

void get_scores(index_t *ht, char** words, int count, index_t *scores);
static void aggregate_scores(void *arg, const char *key, void* ctrs);
static void sum_scores(void *arg, const int key, int count);

all_docs_t sort_pages(counters_t* ctrs);
void docs_delete(all_docs_t all);

static void counters_count(void *arg, const int key, int count);
static void counters_sort(void *arg, const int id, int count);

static void counters_intersect(counters_t *result, counters_t *ctrs);
static void counter_intersect_helper(void *arg, int key, int count);
static void duplicate_counter(void *arg, const int key, int count);

static void print_results(all_docs_t all, char* page_directory);


/**************** main() ****************/

#ifndef UNIT_TEST
int main(int argc, char* argv[])
{
	if (!check_parameters(argc, argv)) {
		exit(1);
	}

	char* pagedir = argv[1];
	char* filename = argv[2];
	index_t *ht = index_load(filename);

	process_query(pagedir, ht);

	index_delete(ht);
}
#endif //UNIT_TEST

/* check_parameters() takes the input from the command line and makes sure
 * they are all valid: directory must be a crawler directory and the index
 * file must be readable. it returns true or false.
 */
bool check_parameters(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "usage: page_directory index_filename\n");
		return false;
	}

	char* page_directory = argv[1];
	if (!is_crawler_directory(page_directory)) {
		return false;
	}

	char* filename = argv[2];
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file is not readable\n");
		return false;
	} else {
		fclose(fp);
	}

	return true;
}

/* process_query() takes the validated page directory and the index and reads
 * and has the framework for running queries from stdin, one per line, until 
 * EOF
 */
void process_query(char* page_directory, index_t* ht)
{
	char* line;
	
	printf("KEY WORDS:> ");
	while ((line = readlinep(stdin)) != NULL) {
		char** words = count_malloc(sizeof(char*) * 20);
		int count = fetch_words(line, words);
		if (count != 0 && words != NULL){
			print_query(words, count);
			if (check_query(words, count)) {
				run_query(ht, words, count, page_directory);
			}
		}
	
		count_free(words);
		count_free(line);
		printf("KEY WORDS:> ");
	}
	
	printf("\n");
}

/* fetch_words() takes the line to find words from and an intialized array to
 * store all the words in that line to. it considers all alpha characters that
 * are consecutive to be strings. all words are normalized before adding to 
 * they array. it returns the number of words found, or 0 if none/error.
 */
int fetch_words(char* line, char** words) 
{
	int size = strlen(line);
	int count = 0;

	int c;
	char* word;
	char* rest;

	int k;
	for (int i = 0; i < size; i++) {
		k = line[i];
		if (!isalpha(k) && !isspace(k)) {
			fprintf(stderr, 
				"Error: bad character '%c' in query\n", k);
			return 0;
		}
	}

	for (int i = 0; i < size; i++) {
		c = line[i];

		if (isalpha(c)) {
			word = &line[i];
			words[count] = NormalizeWord(word);
			count++;
		
		   	int j = i;
		   	int v = line[j];
		    	while (isalpha(v)) {
		        	v = line[j];
		        	j++;
		    	}
		
		 	rest = &line[j-1];
		 	*rest = '\0';			    
		  	i = j-1;
		}
	}		
	return count;
}

/* print_query() prints the input, cleaned up. non alpha characters are 
 * already caught by fetch_words().
 */
void print_query(char** words, int count) 
{
	printf("Query:");

	for (int i = 0; i < count; i++) {
		printf(" %s", words[i]);
	}

	printf("\n");
}

/* check_query() makes sure that query syntax is valid. it takes an array of
 * lowercase words. it returns true if it is valid, false otherwise.
 */
bool check_query(char** words, int count)
{
	
	char* word = words[0];
	char* prev = word;
	
	for (int i = 0; i < count; i++) {
		word = words[i];
		
		// check syntax
		if ((i == 0) && is_literal(word)) {
			fprintf(stderr, "Error: '%s' cannot be first\n", word);
			return false;
		} else if ((i == count - 1) && is_literal(word)) {
			fprintf(stderr, "Error: '%s' cannot be last\n", word);
			return false;
		} else if ((i != 0) && is_literal(prev) && is_literal(word)) {
			fprintf(stderr, 
			 "Error: '%s' and '%s' cannot be adjacent\n", prev, word);
			return false;
		}

		prev = word;
	}
	return true;
}

/* is_literal() takes a string and returns true if it is a literal and false
 * otherwise. literals: and, or
*/
bool is_literal(char* word)
{
		
	if ((strcmp(word, "and") == 0) || (strcmp(word, "or") == 0)) {
		return true;
	} 
	return false;
}

/* run_query() takes the index, words, and count, and actually runs the
 * query.
 */
void run_query(index_t *ht, char** words, int count, char* page_directory)
{
	index_t* and_scores = index_new(200);
	counters_t* final_scores = counters_new();
	
	get_scores(ht, words, count, and_scores);
	assertp(final_scores, "Failed to run query\n");

	index_iterate(and_scores, final_scores, aggregate_scores);
	all_docs_t all = sort_pages(final_scores);
	
	print_results(all, page_directory);
	
	index_delete(and_scores);
	counters_delete(final_scores);
}

/* aggregate_scores() is a helper function that loops through each counter
 * in the index.
 */
static void aggregate_scores(void *arg, const char *key, void* ctrs)
{
	counters_t* final_scores = arg;
	counters_t* current = ctrs;

	counters_iterate(current, final_scores, sum_scores);
}

/* sum_scores() is a helper function that sums all the scores and adds them
 * to the final counter of scores. It loops through individual counters.
 */
static void sum_scores(void *arg, const int key, int count)
{
	counters_t* final_scores = arg;
	if (count != 0) {
		int sum = counters_get(final_scores, key) + count;
		counters_set(final_scores, key, sum);
	}
}

/* duplicate_counter() creates an identical counter arg and is passed to
 * counters_iterate()
 */
static void duplicate_counter(void *arg, const int key, int count)
{
	counters_t* new = arg;
	counters_set(new, key, count);
} 


/* get_scores() returns an index which contains the aggregated scores
 * from all and sequences. it takes the index, the array of words, and count.
 */
void get_scores(index_t *ht, char** words, int count, index_t *scores)
{
	char* word;
	index_t* all_scores = scores;
	//assertp(all_scores, "Failed to intialize index\n");
	
	for (int i = 0; i < count; i++) {
		word = words[i];
		
		// if the word is not and/or
		if (!is_literal(word)) {
			counters_t *orig = index_find(ht, word);	
			counters_t *scores = counters_new();
			
			if (orig != NULL) {
			// duplicate counters
				counters_iterate(orig, scores, duplicate_counter);
			}
				
			int j = i + 1;
			char* next;
			next = words[j];
			while ((j < count) && (strcmp(next, "or") != 0)) {
				
				if (!is_literal(next)) { // not and	
					counters_t *ctrs = index_find(ht, next);
					
					if (ctrs == NULL) {
						ctrs = counters_new();
					}
															counters_intersect(scores, ctrs);
				}
				
				j++;
				if (j < count) {
					next = words[j];
				}
			}

			i = j;
			char str[12];
			char* key = str;
			sprintf(str, "%d", i);
			
			index_insert(all_scores, key, scores);	
		}
	}
}

/* counter_intersect() takes two counters and changes the first one to
 * intersection of the two, where the ID is the same but the key
 * is set to the minimum value.
 */
static void counters_intersect(counters_t *result, counters_t *ctrs)
{
        assertp(result, "Counters intersect error\n");
        assertp(ctrs, "Counters intersect error\n");
	
        two_counters_t counters = { result, ctrs };
        counters_iterate(result, &counters, counter_intersect_helper);
}

/* counter_intersect_helper() asisists counters_intersect()
 */
static void counter_intersect_helper(void *arg, int key, int count)
{
        two_counters_t* counters = arg;

        int num = 0;
        if (count > counters_get(counters->ctrs, key)) {
                num = counters_get(counters->ctrs, key);
        } else {
                num = count;
        }

        counters_set(counters->result, key, num);
}

/* sort_pages() takes a counter and iterates over them to sort in 
 * descending order.
 */
all_docs_t sort_pages(counters_t* ctrs){
	// iterate to find size
	int size = 0;
	counters_iterate(ctrs, &size, counters_count);
	
	doc_t** pages = count_malloc(size * sizeof(doc_t*));
	all_docs_t all = { pages, 0 };
	counters_iterate(ctrs, &all, counters_sort);
	
	return all;
}

/* counter_count() is a helper function that finds the number of
 * counters within a counters structure.
 */
static void counters_count(void *arg, const int key, int count)
{
	int *size = arg;

	if (size != NULL && count != 0) {
		(*size)++;
	}
}

/* counters_sort() uses insertion sort to sort an array of counters.
 */
static void counters_sort(void *arg, const int id, int count)
{
	all_docs_t *all = arg;
	doc_t** pages = all->pages;
	int size = all->size;
	int key, j;

	doc_t *new = (doc_t*)count_malloc(sizeof(doc_t));
	new->key = id;
	new->count = count;

	int i = size;	
	pages[i] = new;
	all->size = size + 1;
	
	key = count;		
	j = i - 1;
	
	while (j >= 0 && pages[j]->count < key) {
		(all->pages)[j+1]->count = (all->pages)[j]->count;
		(all->pages)[j+1]->key = (all->pages)[j]->key;

		j = j - 1;
	}
	(all->pages)[j+1]->count = key;
	(all->pages)[j+1]->key = id;
	(all->pages)[i] = new;
}

/* prints the all_docs struct and tests its functionality
 */
static void print_results(all_docs_t all, char* page_directory)
{
	all_docs_t *everything = &all;
	doc_t** pages = everything->pages;

	int size = everything->size;
	int id, count;
	
	if (size == 0) {
		printf("No documents match.\n");
	} else {
	
		printf("Matches %d documents (ranked):\n", size);
		for (int i = 0; i < size; i++) {
			id = pages[i]->key;
			count = pages[i]->count;

			char* name = make_filename(page_directory, id);
			
			FILE *fp = fopen(name, "r");
			assertp(fp, "Could not open file.\n");
			
			char* url = readlinep(fp);
			assertp(url, "Could not get URL.\n");
			fclose(fp);
			
			printf("score %1d doc%2d: %2s\n", count, id, url);
			
			count_free(url);	
			count_free(name);
			count_free(pages[i]);

		}
	}
	printf("-----------------------------------------------\n");
	count_free(pages);
	
}

/*****************************************************
 ******************** unit testing *******************
 *****************************************************/

#ifdef UNIT_TEST
#include "unittest.h"

////////////////////////////////////////
// attempt to run fetch_words()
int test_fetch_words()
{
	START_TEST_CASE("fetch_words()");

	char first[] = " HELLO  hi i am christina pls work ";
	char** array1  = count_malloc(7 * sizeof(char*));
	int count1 = fetch_words(first, array1);
	
	if (count1 != 0) {
		EXPECT(array1 != NULL);

		for (int i = 0; i < count1; i++) {
			printf("%s\n", array1[i]);
		}	
	}
	
	char second[] = "a second test";
	char** array2 = count_malloc(3 * sizeof(char*));
	int count2 = fetch_words(second, array2);
	
	if (count2 != 0) {
		EXPECT(array2 != NULL);

		for (int i = 0; i < count2; i++) {
			printf("%s\n", array2[i]);
		}
	}

	count_free(array1);
	count_free(array2);
	

	EXPECT(count_net() == 0);

	END_TEST_CASE;
	return TEST_RESULT;
}

////////////////////////////////////////
// tests check_query() and print_query();
int test_check_query()
{
	START_TEST_CASE("check_query()");

	// test 0
	char line0[] = "and then there were none";
	char** words0 = count_malloc(5 * sizeof(char*));
	int count0 = fetch_words(line0, words0);
	print_query(words0, count0);
	EXPECT(!check_query(words0, count0));
	count_free(words0);
	
	// test 1
	char line1[] = "either or";
	char** words1 = count_malloc(2 * sizeof(char*));
	int count1 = fetch_words(line1, words1);
	print_query(words1, count1);	
	EXPECT(!check_query(words1, count1));
	count_free(words1);

	// test 2
	char line2[] = "fight or flight";
	char** words2 = count_malloc(3 * sizeof(char*));
	int count2 = fetch_words(line2, words2);
	print_query(words2, count2);
	EXPECT(check_query(words2, count2));
	count_free(words2);

	// test 3
	char line3[] = "christina and or lu";
	char** words3 = count_malloc(4 * sizeof(char*));
	int count3 = fetch_words(line3, words3);
	print_query(words3, count3);
	EXPECT(!check_query(words3, count3));
	count_free(words3);
	EXPECT(count_net() == 0);

	END_TEST_CASE;
	return TEST_RESULT;
}

////////////////////////////////////////
// tests get_scores()

int test_get_scores()
{
	START_TEST_CASE("get_scores()");

	char line0[] = "playground and home";
	char** words0 = count_malloc(3 * sizeof(char*));
	int count0 = fetch_words(line0, words0);
	print_query(words0, count0);
	EXPECT(check_query(words0, count0));
	
	index_t *ht = index_load("./index3");
	index_t *scores = index_new(100);
	get_scores(ht, words0, count0, scores);
	index_save(scores, "test_result");

	count_free(words0);
	index_delete(ht);
	index_delete(scores);

	END_TEST_CASE;
	return TEST_RESULT;
}

////////////////////////////////////////
// main()

int main(const int argc, const char *argv[])
{
	int failed = 0;

	failed += test_fetch_words();
	failed += test_check_query();
	failed += test_get_scores();

	if (failed) {
		printf("FAIL %d test cases\n", failed);
		return failed;
	} else {
		printf("PASS all test cases\n");
		return 0;
	}
}

#endif // UNIT_TEST
