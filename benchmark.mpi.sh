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
PROCESS_COUNTS=(1 2 4 8)

echo "tamanho_buffer,tempo_execucao,n_processos" > $OUTPUT_FILE

echo "Compilando..."
mpicc -o main src/main.c src/definitions.c src/utils/hash/fnv_hash.c \
    src/utils/hashmap/hashmap.c src/utils/io/read_file.c src/utils/count/count.c \
    -I./src -I./src/utils/hash -I./src/utils/hashmap -I./src/utils/io -I./src/utils/threads

for PROCESSES in "${PROCESS_COUNTS[@]}"; do
    for file in "${TEST_FILES[@]}"; do
        if [ -f "$file" ]; then
            echo "Executando com arquivo $file e $PROCESSES processos..."
            mpirun -np $PROCESSES ./main "$file" >> $OUTPUT_FILE
        else
            echo "Arquivo $file não encontrado, pulando..."
        fi
    done
done

echo "Benchmark concluído. Resultados salvos em $OUTPUT_FILE"
