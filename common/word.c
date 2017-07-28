/*
 * word.c - functions for TSE work with words
 *
 * David Kotz - April 2016, 2017
 */

#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/**************** NormalizeWord ****************/
/*
 * See "word.h" for full documentation.
 */
char *
NormalizeWord (char *word)
{
  if (word != NULL) {
    for (char *p = word; *p != '\0'; p++)
      *p = tolower(*p);
  }
  return word;
}

