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
	echo $(MAIN_LINK)
	#g++ $(CFLAGS) $(SRC_DIR)/main.cpp -o bin/main $(MAIN_LINK)


NCGui:
	# Compiling NCGui
	@$(CC) $(CFLAGS) $(GUI_DIR)/NCGui.cpp -o bin/main $(MAIN_LINK) -lncurses

GUI:
	# Compiling GUI
	@$(CC) $(CFLAGS) $(GUI_DIR)/GUI.cpp -o bin/main $(MAIN_LINK) `pkg-config --libs --cflags gtkmm-3.0`

CPU:
	# Compiling CPU
	@$(CC) $(CFLAGS) $(CPU_DIR)/CPU.cc -c -g -o bin/CPU.o 

Memory:
	# Compiling Memory	
	@$(CC) $(CFLAGS) $(CPU_DIR)/Memory.cc -c -o bin/Memory.o

Disasm:
	# Compiling Disasm
	@$(CC) $(CFLAGS) $(SRC_DIR)/Disasm/Disasm.cc -c -o bin/Disasm.o 
	@$(CC) $(CFLAGS)  $(SRC_DIR)/main.cpp -o bin/main bin/Disasm.o

testbin:
	nasm -f bin $(SRC_DIR)/CPU/testbin.asm -o bin/test.bin

# Tests
TestCPU0:
	# Compiling TestCPU0
	@$(CC) $(CFLAGS) $(TEST_DIR)/TestCPU0.cc -o $(TEST_BIN)/TestCPU0 -lboost_unit_test_framework $(MAIN_LINK)
