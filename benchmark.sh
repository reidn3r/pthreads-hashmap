#!/bin/bash

OUTPUT_FILE="./io/output/benchmark.csv"
TEST_FILES=(
    "io/files/alice_150kb.txt" 
    "io/files/alice_1mb.txt" 
    "io/files/alice_5mb.txt" 
    "io/files/alice_10mb.txt" 
    "io/files/alice_50mb.txt" 
    "io/files/alice_100mb.txt" 
    "io/files/alice_500mb.txt"
    )
THREAD_COUNTS=(1 2 4 8 16 32)

echo "tamanho_buffer,tempo_execucao,n_threads" > $OUTPUT_FILE

for THREADS in "${THREAD_COUNTS[@]}"; do
    echo "Compilando com $THREADS threads..."

    gcc -o main src/main.c src/definitions.c src/utils/hash/fnv_hash.c \
        src/utils/hashmap/hashmap.c src/utils/io/read_file.c src/utils/threads/threads.c \
        -I./src -I./src/utils/hash -I./src/utils/hashmap -I./src/utils/io -I./src/utils/threads \
        -lpthread -DTOTAL_THREADS=$THREADS

    for file in "${TEST_FILES[@]}"; do
        if [ -f "$file" ]; then
            echo "Executando com arquivo $file e $THREADS threads..."
            ./main "$file" >> $OUTPUT_FILE
        else
            echo "Arquivo $file não encontrado, pulando..."
        fi
    done
done

echo "Benchmark concluído. Resultados salvos em $OUTPUT_FILE"
