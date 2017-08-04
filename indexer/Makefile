# Makefile for indexer
#
# Christina Lu, July 2017

# object files
LIBS = ../common/libcommon.a ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make


.PHONY: clean valgrind indexer indextest all

all: indexer indextest

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: ../libcs50/memory.h ../common/index.h ../common/pagedir.h

indextest.o: ../common/index.h 

test: 
	chmod +x test.sh
	./test.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer indextest
	rm -rf data/

