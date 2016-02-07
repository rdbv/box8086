CC=g++
CFLAGS=-std=c++11 -Wall 
LINK= bin/Memory.o bin/CPU.o

BIN_DIR=bin
SRC_DIR=src
CPU_DIR=$(SRC_DIR)/CPU

#tt

all: CPU Memory main

main:
	# Compiling main.cpp
	@$(CC) $(CFLAGS)  $(SRC_DIR)/main.cpp -o bin/main bin/Memory.o bin/CPU.o bin/Disasm.o


GUI:
	# Compiling GUI
	@$(CC) $(CFLAGS) -O3 $(SRC_DIR)/ConsoleGUI/GUI.cpp -o bin/main bin/Memory.o bin/CPU.o bin/Dis.o bin/Out.o -lncurses

CPU:
	# Compiling CPU
	@$(CC) $(CFLAGS) $(CPU_DIR)/CPU.cc -c -o bin/CPU.o 
	@$(CC) $(CFLAGS) $(SRC_DIR)/ConsoleGUI/GUI.cpp bin/CPU.o bin/Memory.o -o bin/main  -lncurses

Memory:
	# Compiling Memory	
	@$(CC) $(CFLAGS) $(CPU_DIR)/Memory.cc -c -o bin/Memory.o
	@$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp bin/CPU.o bin/Memory.o -o bin/main

Disasm:
	$(CC) $(CFLAGS) $(SRC_DIR)/Disasm/Disasm.cc -c -o bin/Disasm.o 
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp bin/Disasm.o -o bin/main


# Tests stuff
testbin:
	nasm -f bin $(CPU_DIR)/testbin.asm -o $(BIN_DIR)/test.bin

bochs:
	nasm -f bin ../bochs.asm -o $(BIN_DIR)/test.bin



