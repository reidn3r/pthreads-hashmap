#ifndef THREADS_H
#define THREADS_H

#include "../hashmap/hashmap.h"
#include "../io/read_file.h"

typedef struct {
    int start;
    int end;
} ProcessArgs;

HashMap* count_words(FileBuffer buffer);

FileBuffer* partition_buffer(FileBuffer buffer, int total_processes);

void merge_maps(HashMap** dest, HashMap* src);

#endif