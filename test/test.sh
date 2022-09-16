#!/bin/bash

TEST_DIR=$(dirname "$0")
PROJECT_DIR=$(dirname "$TEST_DIR")

for bsz in 1 2 3 5 7 16 32 64 1024 1048576
do
	cc -g -target x86_64-apple-macos10.12 -I "$PROJECT_DIR" -D BUFFER_SIZE="$bsz" -o "$TEST_DIR"/test \
		"$TEST_DIR"/test.c "$PROJECT_DIR"/get_next_line.c "$PROJECT_DIR"/get_next_line_utils.c \
		&& "$TEST_DIR"/test "$TEST_DIR"/want.txt "$TEST_DIR"/want.txt
done

rm "$TEST_DIR"/test
