# Makefile for crawler
#
# Christina Lu, July 2017

PROG = crawler
LIBS = ../common/libcommon.a ../libcs50/libcs50.a 

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make

$(PROG): crawler.o $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.h

.PHONY: unittest unit test clean

unittest: crawler.c $(LIBS) 
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
