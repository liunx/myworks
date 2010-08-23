# Makefile

CC = gcc

all: getword copystr return_list
getword: getword.c
	$(CC) -g $< -o $@

copystr: copystr.c
	$(CC) -g $< -o $@

return_list: return_list.c
	$(CC) -g $< -o $@

clean:
	rm -f getword
	rm -f copystr
	rm -f return_list
