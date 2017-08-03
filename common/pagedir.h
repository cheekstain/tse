/*
 * pagedir.h - header file for TSE pagedir module
 *
 * This module contains all the logic for saving and loading pages to and from 
 * a crawler output directory. These methods are used by the crawler and the
 * indexer.
 *
 * Christina Lu, August 2017
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>

/**************** functions ****************/

/* Writes page HTML to file, in the format:
 * 	URL
 * 	depth (int)
 * 	HTML (unmodified)
 * Takes page structure to be saved, page_directory path to save file in, and
 * an id number (int) to name the file. Returns true if file is written and saved
 * properly, false otherwise. 
 */ 
bool page_saver(webpage_t* page, char* page_directory, int id);

