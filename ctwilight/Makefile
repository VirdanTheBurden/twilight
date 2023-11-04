CC=gcc
CLFAGS=-c -I include/ -Wall -Wextra --std=c99
targetFiles := $(wildcard src/*.c) $(wildcard src/*/*.c)
objectFiles := $(wildcard *.o)

all: compile cpipp clean

cpipp: 
	$(CC) $(objectFiles) -o cpipp

compile: $(targetFiles)
	$(CC) $(CLFAGS) $(targetFiles)

clean:
	rm -rf *.o