#include <stdlib.h>
#include <stdio.h>
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


int add(HashMap** map_ptr, char* key) {
  HashMap* map = *map_ptr;
  float load_ratio = (float)map->n_items / map->length;
  if(load_ratio > LOAD_FACTOR) {
    HashMap* new_map = resize(map);
    *map_ptr = new_map;  
    map = new_map;       
  }

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

HashMap* resize(HashMap* map) {
  printf("RESIZED!!!\n");
  HashMap* new_map = (HashMap*) malloc(sizeof(HashMap));
  new_map->buckets = calloc(map->length * 2, sizeof(HashmapEntry*));
  new_map->length = map->length * 2;
  new_map->n_items = 0;

  for(int i=0; i<map->length; i++){
    HashmapEntry* current = map->buckets[i];
    while (current != NULL){
      HashmapEntry* next = current->next;
      
      char* key = current->key;
      int new_idx = __get_bucket_index(key, new_map->length);
      
      current->next = new_map->buckets[new_idx];
      new_map->buckets[new_idx] = current;
      new_map->n_items++;

      current = next;
    }
  }
  free(map->buckets);
  free(map);
  return new_map;
}

int __get_bucket_index(char* key, int hashmap_size) {
  uint32_t hash_key = fnv1a_32(key);
  return hash_key % hashmap_size;
}
