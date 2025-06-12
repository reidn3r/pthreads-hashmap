#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include "utils/hashmap/hashmap.h"
#include "utils/hash/fnv_hash.h"
#include "utils/io/read_file.h"
#include "utils/threads/threads.h"

#define maxThreads 8

struct tipoPack
{ 
    int id;
    int begin;
    int end;
    FileBuffer *buffer;
    HashMap *map;
};

typedef struct tipoPack tipoPack;


int main() {
  tipoPack pack[maxThreads];
  pthread_t thread[maxThreads];
  int iret[maxThreads];
  HashMap* map[maxThreads];

  int interval, nThreads, begin, end, remain;

  FileBuffer buffer = read_file_to_buffer("files/alice.txt");

  printf("\nQual o numero de threads que deseja utilizar: ");
  scanf("%d", &nThreads);

  interval = buffer.size / nThreads;
  remain = buffer.size % nThreads;

  begin = 0;
  end = interval - 1;

  if (remain>0) {
    end++;
    remain--;
  }

  for (int i = 0; i < nThreads; i++) {
    map[i] = init_hashmap();
    pack[i].id = i + 1;
    pack[i].begin = begin;
    pack[i].end = end;
    pack[i].buffer = &buffer;
    pack[i].map = map[i];

    while (end < buffer.size && isalpha(buffer.data[end])) {
      end++;
    }

    iret[i] = pthread_create(&thread[i], NULL, count_words, (void*) &pack[i]);

    begin = end + 1;
    end = begin + interval - 1;

    if (remain>0) {
      end++;
      remain--;
    }
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(thread[i], NULL);
  }
  
  // for (int i = 0; i < map->length; i++) {
  //   HashmapEntry* entry = map->buckets[i];
  //   while(entry != NULL) {
  //     printf("%s - freq.: %d\n", entry->key, entry->count);
  //     entry = entry->next;
  //   }
  // }
  // printf("%s\n", buffer.data);
  printf("Tamanho do buffer: %ld\n", buffer.size);


  free_file_buffer(buffer);

  return 0;
}