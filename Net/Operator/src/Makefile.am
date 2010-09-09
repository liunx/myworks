bin_PROGRAMS = operator

AM_CFLAGS = -pthread -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOUCE \
	    -I/usr/include/apr-1.0 -L/usr/local/lib -lapr-1 -lrt -lcrypt \
	    -Wall

operator_SOURCES = main.c parseconfig.c connection.c \
		   connection.h parseconfig.h
