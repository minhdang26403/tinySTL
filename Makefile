# Makefile
#****************************************************************

CC = gcc
CPP = g++
CFLAGS = -Wall -Wextra -Wno-unused-function -std=c++20
LIBs = -lm
TESTDIR = ./test
INCLUDEDIR = -I./include

PROGRAMS = type_traits \
	typelist \
	memory \
	tuple \
	exprtmpl \
	array \
	vector

all: $(PROGRAMS)

type_traits: $(TESTDIR)/type_traits.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

typelist: $(TESTDIR)/typelist.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

memory:$(TESTDIR)/memory.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

tuple:$(TESTDIR)/tuple.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

exprtmpl:$(TESTDIR)/exprtmpl.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

array:$(TESTDIR)/array.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

vector:$(TESTDIR)/vector.cpp
	$(CPP) $(CFLAGS) $^ -o $@ $(INCLUDEDIR)

clean:
	rm -rf $(PROGRAMS) *.o *.a a.out *.err *~