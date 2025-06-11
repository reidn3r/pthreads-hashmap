#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdlib.h>

typedef struct {
    char* data;
    size_t size;
} FileBuffer;

FileBuffer read_file_to_buffer(const char* file_name);

#endif
