#ifndef HASHMAP_H
#define HASHMAP_H

// #define HASHMAP_BASE_SIZE 48
#define HASHMAP_BASE_SIZE 8
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
HashMap* resize(HashMap* map);
int add(HashMap** map_ptr, char* key);
HashmapEntry* get(HashMap* map, char* key);
int contains(HashMap* map, char* key);
int add_with_count(HashMap** map_ptr, char* key, int value);
int __get_bucket_index(char* key, int hashmap_size);
void print_hashmap(HashMap* map);
void free_hashmap(HashMap* map);

#endif