#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "utils/io/read_file.h"

#define MAX_WORD_LENGTH 256

extern const char FILE_PATH[];
extern FileBuffer buffer;

#ifndef TOTAL_THREADS
#define TOTAL_THREADS 4
#endif

#endif