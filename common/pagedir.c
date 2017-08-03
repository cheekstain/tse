/*
 * pagedir.c - TSE 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Christina Lu, August 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "memory.h"
#include "file.h"
#include "pagedir.h"

/************************ global functions **************************/
/* see pagedir.h for comments about exported functions */

/************************ local functions *************************/

/************************ page_saver() **************************/
bool page_saver(webpage_t* page, char* page_directory, int id)
{
	char* str = make_filename(page_directory, id);
	if (str == NULL) {
		return false;
	}

	// create new file
	FILE *fp = fopen(str, "w");
	if (fp == NULL) {
		fprintf(stderr, "error opening file %d\n", id);
		return false;
	}

	// write to file
	char* url = webpage_getURL(page);
	if (url == NULL) {
		fprintf(stderr, "error writing url for file %d\n", id);
	} else {
		fprintf(fp, "%s\n", url);
	}

	int depth = webpage_getDepth(page);
	if (depth < 0) {
		fprintf(stderr, "error writing depth for file %d\n", id);
	} else {
		fprintf(fp, "%d\n", depth);
	}

	char* html = webpage_getHTML(page);
	if (html == NULL) {
		fprintf(stderr, "error writing html for file %d\n", id);
	} else {
		fprintf(fp, "%s", html);
	}

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file %d\n", id);
	}

	count_free(str);
	return true;
}

/*************** is_crawler_directory() ****************/
bool is_crawler_directory(char* page_directory){
	// create string page_dreictory/.crawler 
	int length = 10 + strlen(page_directory);
	char *str = count_malloc(sizeof(char) * length);
	if (str == NULL) {
		fprintf(stderr, "error allocating string\n");
		exit(1);
	}
	strcpy(str, page_directory);
	strcat(str, "/.crawler");
	
	// attempt to open file
	FILE *fp = fopen(str, "r");
	if (fp == NULL) {
		fprintf(stderr, "invalid directory %s\n", page_directory);
		return false;
	}

	if (fclose(fp) != 0) {
		fprintf(stderr, "error closing file\n");
		return false;
	}

	count_free(str);
	return true;
}

/***************** make_filename() ****************/
char* make_filename(char* page_directory, int id) {
	char* num = count_malloc(sizeof(char) * 10);
	char* str = count_malloc(sizeof(char) * (strlen(page_directory) + 11));
	if (num == NULL || str == NULL) {
		fprintf(stderr, "error allocating filename\n");
		return NULL;
	}

	// copy components into one string
	sprintf(num, "%d", id);
	strcpy(str, page_directory);
	strcat(str, "/");
	strcat(str, num);

	count_free(num);
	return str;
}
