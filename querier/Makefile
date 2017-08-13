
# Makefile for querier
#
# Christina Lu, July 2017

PROG = querier
LIBS = ../common/libcommon.a ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make

$(PROG): querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: ../libcs50/counters.h ../libcs50/memory.h ../common/pagedir.h ../common/index.h ../libcs50/file.h ../common/word.h

.PHONY: unittest unit clean

unittest: querier.c $(LIBS)
	$(CC) $(CFLAGS) -DUNIT_TEST querier.c $(LIBS) -o unittest

unit: unittest
	./unittest

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f unittest
