## CS130 Fall 2013: Computer Graphics
##
## This file does not need to be modified
#########################################

LIBS = 
CC = g++

## Global header files
INCLUDE =

## Object files and executables
MAIN_OUT = puzzle

## Requirements for each command
MAIN_REQS = 8_puzzle.cpp 8_puzzle.h

## Targets to compile for each command
MAIN_TARGETS = 8_puzzle.cpp 8_puzzle.h

all: main

## Main 
main: $(MAIN_REQS)
	$(CC) $(MAIN_TARGETS) $(LIBS) -o $(MAIN_OUT)

clean:
	rm -f *~ *.o *.out
