COMPILER = clang
FLAGS = -std=c99 -Wall -Wextra -Wconversion -Wunreachable-code -Wpointer-arith -O2
LIBS = -lm

SRC = $(wildcard src/*/*.c) $(wildcard src/*.c) 

OUTPUT = .
BINNAME = main

all: 
	$(COMPILER) $(SRC) -o $(OUTPUT)/$(BINNAME) $(LIBS) $(FLAGS)
