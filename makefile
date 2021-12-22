
CC=clang
CFLAGS=-g -Wall

BUILD_FOLDER=build
INPUTS_FOLDER=inputs

.PHONY: clean all

all: day01 day02 day03 day04 day05 day06

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

day02: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt

day03: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt

day04: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt

day05: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt

day06: $(BUILD_FOLDER)/$$@.app
	@echo Running $<
	@$< $(INPUTS_FOLDER)/$@.txt
