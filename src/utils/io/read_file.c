#include "read_file.h"

#include <stdio.h>
#include <stdlib.h>

long get_file_size(FILE* file_stream) {
    fseek(file_stream, 0, SEEK_END);
    long file_size = ftell(file_stream);
    rewind(file_stream);

    return file_size;
}

char* read_file_to_buffer(const char* file_name) {
    FILE* file_stream = fopen(file_name, "r");
    if (file_stream == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    long file_size = get_file_size(file_stream);

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file_stream);
        return NULL;
    }

    size_t read_bytes = fread(buffer, 1, file_size, file_stream);
    buffer[read_bytes] = '\0';

    fclose(file_stream);

    return buffer;
}