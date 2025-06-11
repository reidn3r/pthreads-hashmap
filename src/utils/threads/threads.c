#include "threads.h"
#include <ctype.h>

#define MAX_WORD_LENGTH 256

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