#!/bin/bash

# Script para compilar e executar o programa

# Compilar
echo "Compilando..."
gcc -o main src/main.c src/definitions.c src/utils/hash/fnv_hash.c \
    src/utils/hashmap/hashmap.c src/utils/io/read_file.c \
    src/utils/threads/threads.c -lpthread

# Verificar se a compilação foi bem sucedida
if [ $? -ne 0 ]; then
    echo "Falha na compilação!"
    exit 1
fi

echo "Compilação concluída com sucesso!"

# Executar com o arquivo alice_1mb.txt
echo "Executando com alice_1mb.txt..."
./main ./io/files/alice_1mb.txt

# Verificar se a execução foi bem sucedida
if [ $? -ne 0 ]; then
    echo "Falha na execução!"
    exit 1
fi