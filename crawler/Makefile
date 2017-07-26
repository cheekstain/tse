# Makefile for crawler
#
# Christina Lu, July 2017

# object files
PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50.a

CFLAGS =  -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.h

.PHONY: clean

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f names
