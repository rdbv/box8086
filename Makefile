CC=g++
CFLAGS=-std=c++11 -Wall 
AS=nasm
AFLAGS=-f bin

LINK= bin/Memory.o bin/CPU.o

BIN_DIR=bin
SRC_DIR=src
CPU_DIR=$(SRC_DIR)/CPU
GUI_DIR=$(SRC_DIR)/GUI
DIS_DIR=$(SRC_DIR)/Disasm
TEST_DIR=tests
MAIN_LINK=bin/Memory.o bin/CPU.o bin/Disasm.o
TEST_BIN=$(BIN_DIR)/tests_bin

all: testbin Disasm CPU Memory GUI tests
tests: TestCPU0

#######################################################################

main:
	# Compiling main
	@$(CC) $(CFLAGS)  $(SRC_DIR)/main.cpp -o bin/main $(MAIN_LINK)

NCGui:
	# Compiling NCGui
	@$(CC) $(CFLAGS) $(GUI_DIR)/NCGui.cpp -o bin/main $(MAIN_LINK) -lncurses

GUI:
	# Compiling GUI
	@$(CC) $(CFLAGS) $(GUI_DIR)/GUI.cpp -o bin/main $(MAIN_LINK) `pkg-config --libs --cflags gtkmm-3.0`

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
	@find bin -maxdepth 1 -type f -exec rm -f {} \;
	@rm -f bin/tests_bin/*
	# Cleaned...

# Tests
TestCPU0:
	# Compiling TestCPU0
	@$(CC) $(CFLAGS) $(TEST_DIR)/TestCPU0.cc -o $(TEST_BIN)/TestCPU0 -lboost_unit_test_framework $(MAIN_LINK)

# For easy compiling in vim.

CPU_vim: CPU 
Memory_vim: Memory GUI
Disasm_vim: Disasm main
GUI_vim: GUI 
NCGui_vim: NCGui
testbin_vim: testbin
Opcodes_vim: Disasm main
main_vim: main
bochs_vim: bochs
TestCPU0_vim: TestCPU0

#######################################################################

# Binary tests stuff
testbin:
	# Assembling testbin
	@$(AS) $(AFLAGS) $(CPU_DIR)/testbin.asm -o $(BIN_DIR)/test.bin

bochs:
	nasm -f bin ../bochs.asm -o $(BIN_DIR)/test.bin



