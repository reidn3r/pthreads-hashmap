#include <stdio.h>
#include <stdlib.h>
#include "utils/hashmap/hashmap.h"
#include "utils/hash/fnv_hash.h"
#include "utils/io/read_file.h"
#include "utils/threads/threads.h"

int main() {
  /*
  const char *palavras[] = {"amor", "computador", "casa", "água", "gato", "livro", "noite", "sol", "cachorro",
    "amigo", "felicidade", "carro", "comida", "tempo", "música", "escola", "trabalho", "viagem", "dia", "janela",
    "cidade", "amor", "sol", "cachorro", "livro", "noite", "computador", "água", "gato", "casa",
    "amigo", "felicidade", "comida", "tempo", "carro", "música", "escola", "trabalho", "viagem", "janela",
    "dia", "cidade", "amor", "computador", "casa", "livro", "noite", "sol", "água", "gato",
    "cachorro", "amigo", "felicidade", "comida", "tempo", "carro", "música", "escola", "trabalho", "viagem",
    "janela", "dia", "cidade", "amor", "computador", "casa", "livro", "noite", "sol", "água",
    "gato", "cachorro", "amigo", "felicidade", "comida", "tempo", "carro", "música", "escola", "trabalho",
    "viagem", "janela", "dia", "cidade", "amor", "computador", "casa", "livro", "noite", "sol",
    "água", "gato", "cachorro", "amigo", "felicidade", "comida", "tempo", "carro", "música", "escola",
    "trabalho", "viagem", "janela", "dia", "cidade", "amor", "computador", "casa", "livro", "noite",
    "sol", "água", "gato", "cachorro", "amigo", "felicidade", "comida", "tempo", "carro", "música",
    "escola", "trabalho", "viagem", "janela", "dia", "cidade", "amor", "computador", "casa", "livro",
    "noite", "sol", "água", "gato", "cachorro", "amigo", "felicidade", "comida", "tempo", "carro",
    "música", "escola", "trabalho", "viagem", "janela", "dia", "cidade", "amor", "computador", "casa",
    "livro", "noite", "sol", "água", "gato", "cachorro", "amigo", "felicidade", "comida", "tempo",
    "carro", "música", "escola", "trabalho", "viagem", "janela", "dia", "cidade"};
    
    for(int i=0; i<100; i++){
      add(&map, (char*)palavras[i]);
    }
  */
  FileBuffer buffer = read_file_to_buffer("files/cr7.txt");

  printf("%s\n", buffer.data);
  printf("Tamanho do buffer: %ld\n", buffer.size);

  HashMap* map = init_hashmap();
  
  count_words(buffer, map);

  free_file_buffer(buffer);
  
  for(int i = 0; i < map->length; i++) {
    HashmapEntry* entry = map->buckets[i];
    while(entry != NULL) {
      printf("%s - freq.: %d\n", entry->key, entry->count);
      entry = entry->next;
    }
  }

  return 0;
}