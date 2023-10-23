CC=gcc
CLFAGS=-c -I include/ -Wall -Wextra 
targetFiles := $(wildcard src/*.c) $(wildcard src/*/*.c)
objectFiles := $(wildcard *.o)

all: cpipp

cpipp: compile
	$(CC) $(objectFiles) -o cpipp

compile: $(targetFiles)
	$(CC) $(CLFAGS) $(targetFiles)

clean:
	rm -rf *.o