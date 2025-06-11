#ifndef HASHMAP_H
#define HASHMAP_H

#define HASHMAP_BASE_SIZE 48
#define LOAD_FACTOR 0.75

/* 
  1. Hashmap do tipo <K:string, V:int>
    1.1 K: Termo
    1.2 V: Frequência
*/

typedef struct HashmapEntry {
  char* key;
  int count;
  struct HashmapEntry* next;
} HashmapEntry;

typedef struct HashMap {
  int length;
  int n_items;
  HashmapEntry** buckets;
} HashMap;


HashMap* init_hashmap();
int add(HashMap* map, char* key);
HashmapEntry* get(HashMap* map, char* key);
int contains(HashMap* map, char* key);
int __get_bucket_index(char* key, int hashmap_size);

#endif