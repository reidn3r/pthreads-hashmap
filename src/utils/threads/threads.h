#ifndef THREADS_H
#define THREADS_H

#include "../hashmap/hashmap.h"
#include "../io/read_file.h"

void count_words(FileBuffer buffer, HashMap* map);

#endif
