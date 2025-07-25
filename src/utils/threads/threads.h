#ifndef THREADS_H
#define THREADS_H

#include "../hashmap/hashmap.h"
#include "../io/read_file.h"

typedef struct {
    int id;
    int start;
    int end;
} ProcessArgs;

HashMap* count_words(FileBuffer buffer);

ProcessArgs* build_mpi_args(FileBuffer buffer, int total_threads);

void* count_words_troutine(void* ptr);

HashMap* mpi_count_words(ProcessArgs args);

void merge_maps(HashMap** dest, HashMap* src);

#endif