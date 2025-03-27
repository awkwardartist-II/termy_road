
# editeable build options:
SRC=./src
CC=g++
ASM=nasm
TARGET=./troad
FLAGS=-m32 -w -fpermissive -I./include -lncurses -lpthread
SFLAGS=-felf32 -I./include

# Find all source files
AllSourceFiles=$(shell find $(SRC) -name '*.c' -o -name '*.cc' -o -name '*.s')

# parse object paths from sources
StdObjectFiles=$(AllSourceFiles:.cc=.o) 	# parse standard C objects
CppObjectFiles=$(StdObjectFiles:.c=.o) 		# parse extended C++ objects
AllObjectFiles=$(CppObjectFiles:.s=.o) 		# parse assembly objects

all: build clean

# links ALL object files
build: $(AllObjectFiles)
	$(CC) $(FLAGS) -o $(TARGET) $(AllObjectFiles)

# compile ALL object files
$(SRC)/%.o: $(SRC)/%.s
	$(ASM) $(SFLAGS) -o $@ $<
$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(FLAGS) -c -o $@ $< 
$(SRC)/%.o: $(SRC)/%.cc
	$(CC) $(FLAGS) -c -o $@ $< 

# removes all built object files
# and prepares for next build
clean:
	rm -f $(shell find $(SRC) -name '*.o')

test:
	$(TARGET)

.PHONY: build test clean
