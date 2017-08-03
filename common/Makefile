# Makefile for common directory
# 	builds and compiles all these functions common to TSE to a library.
# 	based on Makefile for CS50 library
#
# Christina Lu, August 2017

# object files, and the target library
OBJS = index.o pagedir.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)
# dependencies
index.o: index.h
pagedir.o: pagedir.h
word.o: word.h

.PHONY: clean sourcelist 

# list all the sources and docs in this directory
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
