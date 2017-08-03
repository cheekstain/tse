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
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "file.h"

/************************ global functions **************************/
/* see pagedir.h for comments about exported functions */

/************************ local functions *************************/

/************************ page_saver() **************************/
bool page_saver(webpage_t* page, char* page_directory, int id)
{
    // create file name
    char* num = count_malloc(sizeof(char) * 10);
    char* str = count_malloc(sizeof(char) * (strlen(page_directory) + 11));
    if (num == NULL || str == NULL) {
        fprintf(stderr, "error allocating for page_saver\n");
        return false;
    }
    sprintf(num, "%d", id);
    strcpy(str, page_directory);
    strcat(str, "/");
    strcat(str, num);

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
    count_free(num);
    return true;
}
