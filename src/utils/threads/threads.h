#ifndef THREADS_H
#define THREADS_H

#include "../hashmap/hashmap.h"
#include "../io/read_file.h"

typedef struct {
    int id;
    int start;
    int end;
} ThreadArgs;

HashMap* count_words(FileBuffer buffer);

ThreadArgs* build_thread_args(FileBuffer buffer, int total_threads);

void* count_words_troutine(void* ptr);

void merge_maps(HashMap** dest, HashMap* src);

#endif