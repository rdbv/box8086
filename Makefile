CC=g++
CFLAGS=-std=c++11 -Wall 
AS=nasm
AFLAGS=-f bin

LINK= bin/Memory.o bin/CPU.o

BIN_DIR=bin
SRC_DIR=src
CPU_DIR=$(SRC_DIR)/CPU
CGUI_DIR=$(SRC_DIR)/ConsoleGUI
DIS_DIR=$(SRC_DIR)/Disasm
MAIN_LINK=bin/Memory.o bin/CPU.o bin/Disasm.o

all: testbin Disasm CPU Memory GUI

#######################################################################

main:
	# Compiling main
	@$(CC) $(CFLAGS)  $(SRC_DIR)/main.cpp -o bin/main $(MAIN_LINK)

GUI:
	# Compiling GUI
	@$(CC) $(CFLAGS) $(CGUI_DIR)/GUI.cpp -o bin/main $(MAIN_LINK) -lncurses

CPU:
	# Compiling CPU
	@$(CC) $(CFLAGS) $(CPU_DIR)/CPU.cc -c -o bin/CPU.o 

Memory:
	# Compiling Memory	
	@$(CC) $(CFLAGS) $(CPU_DIR)/Memory.cc -c -o bin/Memory.o

Disasm:
	# Compiling Disasm
	@$(CC) $(CFLAGS) $(SRC_DIR)/Disasm/Disasm.cc -c -o bin/Disasm.o 

clean:
	@rm -f bin/*
	# Cleaned...

# For easy compiling in vim.

CPU_vim: CPU GUI
Memory_vim: Memory GUI
Disasm_vim: Disasm main

#######################################################################

# Binary tests stuff
testbin:
	# Assembling testbin
	$(AS) $(AFLAGS) $(CPU_DIR)/testbin.asm -o $(BIN_DIR)/test.bin

bochs:
	nasm -f bin ../bochs.asm -o $(BIN_DIR)/test.bin



