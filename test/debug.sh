#!/bin/sh

BUFFER_SIZE=1
PROJECT_DIR=/Users/joao.lourenco/scroll/github/42.rio_get_next_line
TEST_DIR="$PROJECT_DIR"/test
cc -g -target x86_64-apple-macos10.12 -I "$PROJECT_DIR" -D BUFFER_SIZE="$BUFFER_SIZE" -o "$TEST_DIR"/test \
	"$TEST_DIR"/test.c "$PROJECT_DIR"/get_next_line_bonus.c "$PROJECT_DIR"/get_next_line_utils_bonus.c \
	&& lldb "$TEST_DIR"/test -- "$TEST_DIR"/want.txt "$TEST_DIR"/input_01.txt "$TEST_DIR"/input_02.txt
		"$TEST_DIR"/input_03.txt "$TEST_DIR"/input_04.txt "$TEST_DIR"/input_05.txt \
		"$TEST_DIR"/input_06.txt "$TEST_DIR"/input_07.txt "$TEST_DIR"/input_08.txt \
		"$TEST_DIR"/input_09.txt "$TEST_DIR"/input_10.txt "$TEST_DIR"/input_11.txt \
		"$TEST_DIR"/input_12.txt