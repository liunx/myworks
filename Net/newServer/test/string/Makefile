# Makefile

CC = gcc

all: getword copystr
getword: getword.c
	$(CC) -g $< -o $@

copystr: copystr.c
	$(CC) -g $< -o $@

clean:
	rm -f getword
	rm -f copystr
