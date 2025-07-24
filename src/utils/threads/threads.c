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

ProcessArgs* build_mpi_args(FileBuffer buffer, int total_process) {
    ProcessArgs* process_args = malloc(sizeof(ProcessArgs) * total_process);

    int interval_size = buffer.size / total_process;
    int current_start = 0;
    int current_end = interval_size;

    for (int i = 0; i < total_process; i++) {
        process_args[i].id = i + 1;

        process_args[i].start = current_start;

        while (current_end < (int) buffer.size &&
               isalpha(buffer.data[current_end]))
            current_end++;
        
        if (current_end > (int) buffer.size)
            current_end = buffer.size;

        process_args[i].end = current_end;

        current_start = process_args[i].end;
        current_end += interval_size;
    }

    return process_args;
}

void* count_words_troutine(void* ptr) {
    HashMap* map = init_hashmap();

    ProcessArgs* args = (ProcessArgs*) ptr;
    char current_word[MAX_WORD_LENGTH];
    int word_pos = 0;

    for (int i = args->start; i < args->end; i++) {
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

void merge_maps(HashMap** dest, HashMap* src) {
    for (int i = 0; i < src->length; i++) {
        HashmapEntry* entry = src->buckets[i];
        while (entry) {
            add_with_count(dest, entry->key, entry->count);
            entry = entry->next;
        }
    }
}
