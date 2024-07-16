#!/bin/sh
set -e
gcc -fprofile-arcs -ftest-coverage -O0 -o test_uuid7.out test_uuid7.c src/uuid7.c
./test_uuid7.out
gcov -pb uuid7.c
