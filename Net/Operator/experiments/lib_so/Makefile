CC = gcc
SO_FLAGS = -g -fpic -shared 
CCFLAGS = -g -ldl

all: hello.so hello

hello.so: hello.c
	$(CC) $(SO_FLAGS) hello.c -o libhello.so

hello: main.c
	$(CC) $(CCFLAGS) main.c -o hello

clean:
	rm libhello.so
	rm hello
