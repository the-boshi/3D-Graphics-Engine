CC = gcc
OBJ_NAME = engine
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib
COMPILER_FLAGS = -std=c11 -Wall -O0 -g
LINKER_FLAGS = -lsdl2

SRC = src
BIN = bin
OBJ = obj

SRCS_C := $(shell find $(SRC) -name *.c)
SRCS_O := $(patsubst %.c, %.o, $(SRCS_C))
OBJ_FOR_CLEAN := $(shell find ./src -name *.o)

all: $(SRCS_O)
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(SRCS_O) -o $(BIN)/$(OBJ_NAME)

$(OBJ)/%.o: %.c
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $^ -o $@

clean:
	rm -rf $(OBJ_FOR_CLEAN)
	rm -rf $(BIN)/*

test:
	@echo "This makefile is in the $(THIS_DIR) directory"

