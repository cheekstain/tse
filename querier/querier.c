/* 
 * querier.c - TSE 'querier' module
 *
 * see querier.h for more information.
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
/* none */

/**************** functions ****************/
bool check_parameters(int argc, char* argv[]);
void process_query(char* page_directory, index_t* ht);
void check_query(char** words);
void fetch_words(char* line, char** words);

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
		fprintf(stderr, "usage: page_directory index_filename");
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

	while ((line = readlinep(stdin)) != NULL) {
		char** words = count_malloc(sizeof(char*) * strlen(line));
		fetch_words(line, words);

		if (words != NULL) {
			for (int i = 0; i < 5; i++) {
				printf("%s\n", words[i]);	
			}
			//check_query(words);
			//print_query(words);
			
			//run_query(words);
		}

		count_free(words);
		count_free(line);
		
	}
}

/* fetch_words() takes the line to find words from and an intialized array to
 * store all the words in that line to. it considers all alpha characters that
 * are consecutive to be strings.
 */
void fetch_words(char* line, char** words) 
{
	int size = strlen(line);
	int count = 0;

	int c;
	char* word;
	char* rest;

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
}

void check_query(char** words)
{

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

	char line[] = " HELLO  hi i am christina pls!work";
	
	char** words = count_malloc(strlen(line) * sizeof(char*));
	fetch_words(line, words);
	EXPECT(words != NULL);

	for (int i = 0; i < 7; i++) {
		printf("%s\n", words[i]);
	}

	count_free(words);

	EXPECT(count_net() == 0);

	END_TEST_CASE;
	return TEST_RESULT;
}

////////////////////////////////////////
// main()

int main(const int argc, const char *argv[])
{
	int failed = 0;

	failed += test_fetch_words();

	if (failed) {
		printf("FAIL %d test cases\n", failed);
		return failed;
	} else {
		printf("PASS all test cases\n");
		return 0;
	}
}

#endif // UNIT_TEST
