# Makefile
#****************************************************************

CC = gcc
CPP = g++
CFLAGS = -Wall -Wno-unused-function -std=c++20
LIBs = -lm
TESTDIR = ./test
INCLUDEDIR = -I./include -I.

PROGRAMS = type_traits \
	typelist \
	unique_ptr \

all: $(PROGRAMS)

type_traits: type_traits.cpp
	$(CPP) $(CFLAGS) -o $@ $^ $(INCLUDEDIR) $(LIBS)

typelist: typelist.cpp
	$(CPP) $(CFLAGS) -o $@ $^ $(INCLUDEDIR) $(LIBS)

unique_ptr: unique_ptr.cpp
	$(CPP) $(CFLAGS) -o $@ $^ $(INCLUDEDIR) $(LIBS)

clean:
	rm -rf $(PROGRAMS) *.o *.a a.out *.err *~