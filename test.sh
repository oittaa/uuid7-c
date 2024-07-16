#!/bin/sh
set -e
gcc -Wall -Wextra -o test_uuid7.out src/uuid7.c test_uuid7.c
./test_uuid7.out
