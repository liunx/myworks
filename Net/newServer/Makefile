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

all: newServer server client epoll_testServer


newServer:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) newServer.c -o bin/newServer

server:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) server.c -o bin/server

client:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) client.c -o bin/client
MultiServers:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) MultiServers.c -o bin/MultiServers


epoll_testServer: epoll_testServer.cpp
	g++ epoll_testServer.cpp -o bin/epoll_testServer



clean:
	rm -rf bin/newServer
	rm -rf bin/server
	rm -rf bin/client
	rm -rf bin/epoll_testServer
	rm -rf bin/MultiServers
