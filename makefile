
CC=clang
CFLAGS=-g -Wall

BUILD_FOLDER=build
INPUTS_FOLDER=inputs

.PHONY: clean all

all: day01

clean:
	@echo Clean: removing files from ./$(BUILD_FOLDER) folder
	@rm -rf $(BUILD_FOLDER)/*

$(BUILD_FOLDER)/%.app : %.c aoc_common.h
	@echo Building $@
	@$(CC) $(CFLAGS) -I. $< -o $@

.SECONDEXPANSION:

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

day01: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt
