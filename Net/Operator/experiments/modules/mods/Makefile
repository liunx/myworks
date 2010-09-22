# A simple Makefile for apr sample code.
# for f in *.c; do make -f Makefile.simple `expr $f : '\([^.]*\)\.c'`; done

APR_CONFIG=$(shell which apr-1-config)

CC=gcc
CXX=g++
CFLAGS=$(shell ${APR_CONFIG} --cflags --cppflags --includes) -Wall
CFLAGS += -fpic -shared
CXXFLAGS=$(shell ${APR_CONFIG} --cflags --cppflags --includes) -Wall

LDFLAGS=$(shell ${APR_CONFIG} --ldflags)
LDLIBS=$(shell ${APR_CONFIG} --libs --link-ld)

.PHONY: all clean

all: libtest.so

libtest.so:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) mod_test.c -o libtest.so


clean:
	rm -rf libtest.so
