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

typedef enum {
    ARGS = 0,
    MAPS = 1,
} Tags;

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
    
    if (id == 0) {
        HashMap* final_map;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        buffer = read_file_to_buffer("./io/files/alice_10mb.txt");
        printf("%ld,", buffer.size);
        ProcessArgs* args = build_mpi_args(buffer, numProcess);
    
        for (int i=1; i<numProcess; i++) {
            MPI_Send(&args[i], 2, MPI_INT, i, ARGS, MPI_COMM_WORLD);
        }

        for (int i=1; i<=numProcess; i++) {
            HashMap* partial_map;
            MPI_Recv(partial_map, 3, MPI_INT, i, MAPS, MPI_COMM_WORLD, &status);
            merge_maps(&final_map, partial_map);
            free(partial_map);
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

        MPI_Recv(&args, 2, MPI_INT, 0, ARGS, MPI_COMM_WORLD, &status);

        HashMap* partial_map = mpi_count_words(args);

        MPI_Send(partial_map, 3, MPI_INT, 0, MAPS, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}