#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>  
#include "definitions.h"
#include "utils/hash/fnv_hash.h"
#include "utils/hashmap/hashmap.h"
#include "utils/io/read_file.h"
#include "utils/threads/threads.h"
#include <mpi.h>

#define FLAG_ARGS 1
#define FLAG_MAPS 2

int main(int argc, char *argv[]) {
    int id, numProcess = 0;
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&numProcess);
    MPI_Status status;
    
    HashMap* final_map, *partial_map;
    if (id == 0) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        buffer = read_file_to_buffer("./io/files/alice_10mb.txt");
        printf("%ld,", buffer.size);
        ProcessArgs* args = build_mpi_args(buffer, numProcess);
    
        for(int i=1; i<numProcess; i++){
            MPI_Send(&args[i], 2, MPI_INT, i, FLAG_ARGS, MPI_COMM_WORLD);
        }

        for(int i=1; i<=numProcess; i++){
            MPI_Recv(partial_map, 3, MPI_INT, i, FLAG_MAPS, MPI_COMM_WORLD, &status);
            merge_maps(&final_map, partial_map);
        }

        free_file_buffer(buffer);
        print_hashmap(final_map);
        free_hashmap(final_map);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
    
        double elapsed_time = (end.tv_sec - start.tv_sec) + 
            (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("%.6f,%d\n", elapsed_time, TOTAL_THREADS);
    } else {
        ProcessArgs args; 

        MPI_Recv(&args, 2, MPI_INT, 0, FLAG_ARGS, MPI_COMM_WORLD, &status);

        HashMap* partial_map = count_words_troutine((void *) args);

        MPI_Send(partial_map, 3, MPI_INT, 0, FLAG_MAPS, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}