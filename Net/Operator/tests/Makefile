# A simple Makefile for apr sample code.
# for f in *.c; do make -f Makefile.simple `expr $f : '\([^.]*\)\.c'`; done

APR_CONFIG=$(shell which apr-1-config)

CC=gcc
CXX=g++
CFLAGS=$(shell ${APR_CONFIG} --cflags --cppflags --includes) -Wall
CXXFLAGS=$(shell ${APR_CONFIG} --cflags --cppflags --includes) -Wall

LDFLAGS=$(shell ${APR_CONFIG} --ldflags)
LDLIBS=$(shell ${APR_CONFIG} --libs --link-ld)

.PHONY: all clean

all: parseconfig 

parseconfig:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) parseconfig.c -o parseconfig


clean:
	rm -rf parseconfig
