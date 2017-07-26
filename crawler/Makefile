# Makefile for crawler
#
# Christina Lu, July 2017

# object files
PROG = crawler
OBJS = crawler.o ../libcs50/hashtable.o ../libcs50/bag.o ../libcs50/webpage.o ../libcs50/memory.o
LIBS = ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
MAKE = make

$(PROG): $(OBJS) ../libcs50/libcs50.a 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.h
hashtable.o: ../libcs50/hashtable.h
bag.o: ../libcs50/bag.h
webpage.o: ../libcs50/webpage.h
memory.o: ../libcs50/memory.h

.PHONY: clean

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
