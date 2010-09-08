bin_PROGRAMS = operator
AM_CFLAGS = -pthread -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOUCE \
	    -I/usr/local/apr/include/apr-1 -L/usr/local/apr/lib -lapr-1 -lrt -lcrypt \
	    -Wall
operator_CFLAGS = $(AM_CFLAGS)
operator_SOURCES = main.c
