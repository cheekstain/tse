# Makefile for crawler
#
# Christina Lu, July 2017

PROG = crawler
OBJS = ../libcs50/hashtable.o ../libcs50/bag.o ../libcs50/webpage.o ../libcs50/memory.o ../libcs50/jhash.o ../libcs50/set.o ../libcs50/file.o
LIBS = ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -L../libcs50
MAKE = make

$(PROG): crawler.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o crawler

crawler.o: crawler.h
 hashtable.o: ../libcs50/hashtable.h
 bag.o: ../libcs50/bag.h
 webpage.o: ../libcs50/webpage.h
 memory.o: ../libcs50/memory.h
 jhash.o: ../libcs50/jhash.h
 set.o: ../libcs50/set.h
 file.o: ../libcs50/file.h

.PHONY: unittest unit test clean

unittest: crawler.h crawler.c $(OBJS) unittest.h
	$(CC) $(CFLAGS) -DUNIT_TEST crawler.c $(OBJS) -o unittest

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
