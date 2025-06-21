#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>  
#include "definitions.h"
#include "utils/hash/fnv_hash.h"
#include "utils/hashmap/hashmap.h"
#include "utils/io/read_file.h"
#include "utils/threads/threads.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }
    
    buffer = read_file_to_buffer(argv[1]);
    printf("%ld,", buffer.size);  

    pthread_t threads[TOTAL_THREADS];
    ThreadArgs* thread_args = build_thread_args(buffer, TOTAL_THREADS);
    HashMap* result_maps[TOTAL_THREADS];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TOTAL_THREADS; i++)
        pthread_create(&threads[i], NULL, count_words_troutine, &thread_args[i]);
    
    for (int i = 0; i < TOTAL_THREADS; i++) 
        pthread_join(threads[i], (void**)&result_maps[i]);

    HashMap* final_map = init_hashmap();
    for (int i = 0; i < TOTAL_THREADS; i++) {
        merge_maps(&final_map, result_maps[i]);
        free_hashmap(result_maps[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec) + 
        (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f,%d\n", elapsed_time, TOTAL_THREADS);

    free_file_buffer(buffer);
    free(thread_args);
    free_hashmap(final_map);
    
    return 0;
}