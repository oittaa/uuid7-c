#!/bin/sh
set -e
gcc -Wall -Wextra -fprofile-arcs -ftest-coverage -O0 -o test_uuid7.out -I./src/ test_uuid7.c src/uuid7.c
gcc -Wall -Wextra -fprofile-arcs -ftest-coverage -O0 -o test_threads.out -I./src/ test_threads.c src/uuid7.c
./test_uuid7.out
./test_threads.out
gcov -pb uuid7.c
