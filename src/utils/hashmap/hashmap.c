#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "../hash/fnv_hash.h"

HashMap* init_hashmap() {
  HashMap* map = (HashMap*) malloc(sizeof(HashMap));
  map->buckets = calloc(HASHMAP_BASE_SIZE, sizeof(HashmapEntry*));
  map->length = HASHMAP_BASE_SIZE;
  map->n_items = 0;
  return map;
}


int add(HashMap* map, char* key) {
  //Implementar Resize automático com base em LOAD FACTOR?
  int idx = __get_bucket_index(key, map->length);

  HashmapEntry* current = map->buckets[idx];
  while (current != NULL) {
      if (strcmp(current->key, key) == 0) {
          current->count++;
          return 1;
      }
      current = current->next;
  }

  HashmapEntry* new_entry = (HashmapEntry*)malloc(sizeof(HashmapEntry));
  new_entry->key = strdup(key);  
  new_entry->count = 1;
  new_entry->next = map->buckets[idx];  

  map->buckets[idx] = new_entry;
  map->n_items++;

  return 1;
}

HashmapEntry* get(HashMap* map, char* key) {
  int idx = __get_bucket_index(key, map->length);
  HashmapEntry* current = map->buckets[idx];
  
  while(current != NULL) {
      if(strcmp(current->key, key) == 0) {
          return current;
      }
      current = current->next;
  }
  return NULL;
}

int contains(HashMap* map, char* key) {
  int idx = __get_bucket_index(key, map->length);
  HashmapEntry* current = map->buckets[idx];
  
  while(current != NULL) {
      if (strcmp(current->key, key) == 0) {
          return 1;
      }
      current = current->next;
  }
  return 0;
}


int __get_bucket_index(char* key, int hashmap_size) {
  uint32_t hash_key = fnv1a_32(key);
  return hash_key % hashmap_size;
}
