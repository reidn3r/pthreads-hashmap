#!/usr/bin/bash
set -e

DEBUG=0

# Check for --debug flag
if [[ "$1" == "--debug" ]]; then
  DEBUG=1
  echo "Debug mode enabled"
fi

echo "Compiling C code..."
CFLAGS="-Wall -Wextra"
if [[ $DEBUG -eq 1 ]]; then
  CFLAGS="$CFLAGS -fsanitize=address -g -DDEBUG"
fi

cc $CFLAGS -lpthread $(find src -name '*.c') -o a.out

echo "Executing C code..."
if [[ $DEBUG -eq 1 ]]; then
  gdb ./a.out
else
  ./a.out
fi

rm a.out

echo -e "\nScript ran successfully."
