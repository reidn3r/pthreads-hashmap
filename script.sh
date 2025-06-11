#!/usr/bin/bash
set -e

echo "Compiling C code..."
cc -lpthread -Wall -Wextra $(find src -name '*.c') -o a.out

echo "Executing C code..."
./a.out

rm a.out

echo -e "\nScript ran successfully."