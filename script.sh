#!/bin/bash

DEBUG=false

if [ "$1" == "--debug" ] || [ "$1" == "-d" ]; then
    DEBUG=true
    shift
fi

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $0 [--debug] [num_processes] [input_file]"
    exit 1
fi

NUM_PROCS="$1"
INPUT_FILE="$2"

echo "Compiling..."
if $DEBUG; then
    echo "[DEBUG] mpicc -g -Wall -fsanitize=address $(find src -name '*.c') -o a.out"
    mpicc -g -Wall -fsanitize=address $(find src -name "*.c") -o a.out
else
    mpicc $(find src -name "*.c") -o a.out
fi

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 2
fi

echo "Running a.out with $NUM_PROCS processes on input: $INPUT_FILE..."
if $DEBUG; then
    echo "[DEBUG] mpirun --oversubscribe -np $NUM_PROCS ./a.out $INPUT_FILE"
fi

mpirun --oversubscribe -np "$NUM_PROCS" ./a.out "$INPUT_FILE"

if ! $DEBUG; then
    rm -f a.out
else
    echo "[DEBUG] Skipping a.out cleanup for inspection."
fi
