#ifndef COUNT_H
#define COUNT_H

#include "../hashmap/hashmap.h"
#include "../io/read_file.h"
#include "../../definitions.h"

typedef struct {
    char key[MAX_WORD_LENGTH];
    int count;
} FlatEntry;

HashMap* count_words(FileBuffer buffer);

FileBuffer* partition_buffer(FileBuffer buffer, int total_processes);

FlatEntry* serialize_map(HashMap* map);

void merge_maps(HashMap** dest, HashMap* src);

#endif