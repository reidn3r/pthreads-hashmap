#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "definitions.h"
#include "utils/hash/fnv_hash.h"
#include "utils/hashmap/hashmap.h"
#include "utils/io/read_file.h"
#include "utils/threads/threads.h"

int main() {
  buffer = read_file_to_buffer(FILE_PATH);

  printf("Tamanho do buffer: %ld\n", buffer.size);

  pthread_t threads[TOTAL_THREADS];
  ThreadArgs* thread_args = build_thread_args(buffer, TOTAL_THREADS);
  HashMap* result_maps[TOTAL_THREADS];

  for (int i = 0; i < TOTAL_THREADS; i++)
    pthread_create(&threads[i], NULL, count_words_troutine, &thread_args[i]);
  
  for (int i = 0; i < TOTAL_THREADS; i++) 
    pthread_join(threads[i], (void**)&result_maps[i]);

  HashMap* final_map = init_hashmap();

  for (int i = 0; i < TOTAL_THREADS; i++) {
    merge_maps(&final_map, result_maps[i]);
    free_hashmap(result_maps[i]);
  }

  print_hashmap(final_map);

  free_file_buffer(buffer);
  free(thread_args);
  free_hashmap(final_map);

  return 0;
}