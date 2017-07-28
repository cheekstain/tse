# Makefile for crawler
#
# Christina Lu, July 2017

PROG = crawler
LIBS = ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -L../libcs50
MAKE = make

$(PROG): crawler.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o crawler

crawler.o: crawler.h

.PHONY: unittest unit test clean

unittest: crawler.h crawler.c $(LIBS) unittest.h
	$(CC) $(CFLAGS) -DUNIT_TEST crawler.c $(LIBS) -o unittest

unit: unittest
	mkdir unit_data/
	./unittest

test: crawler
	chmod +x test.sh
	./test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f unittest
	rm -rf unit_data/
	rm -rf test_data/
