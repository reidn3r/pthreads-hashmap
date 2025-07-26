#include "threads.h"
#include <ctype.h>
#include <stdio.h>
#include "../../definitions.h"

HashMap* count_words(FileBuffer buffer) {
    HashMap* map = init_hashmap();

    char current_word[MAX_WORD_LENGTH];
    int word_pos = 0;

    for (size_t i = 0; i < buffer.size; i++) {
        char current_char = buffer.data[i];

        if (isalpha(current_char)) 
            current_word[word_pos++] = tolower(current_char);
        else {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                add(&map, current_word);
                word_pos = 0;
            }
        }
    }

    if (word_pos > 0) {
        current_word[word_pos] = '\0';
        add(&map, current_word);
    }

    return map;
}

FileBuffer* partition_buffer(FileBuffer buffer, int total_processes) {
    FileBuffer* buffer_partitions = malloc(sizeof(FileBuffer) * total_processes);

    int interval_size = buffer.size / total_processes;
    int current_start = 0;
    int current_end = interval_size;

    for (int i = 0; i < total_processes; i++) {
        buffer_partitions[i].data = &(buffer.data[current_start]);

        while (current_end < (int) buffer.size &&
               isalpha(buffer.data[current_end]))
            current_end++;
        
        if (current_end > (int) buffer.size)
            current_end = buffer.size;

        buffer_partitions[i].size = current_end - current_start;

        current_start = current_end;
        current_end += interval_size;
    }

    return buffer_partitions;
}

void merge_maps(HashMap** dest, HashMap* src) {
    for (int i = 0; i < src->length; i++) {
        HashmapEntry* entry = src->buckets[i];
        while (entry) {
            add_with_count(dest, entry->key, entry->count);
            entry = entry->next;
        }
    }
}
