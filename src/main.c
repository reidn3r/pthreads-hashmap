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
    PARTITION_SIZE = 1,
    PARTITION_PTR = 2,
    MAPS = 3,
} Tags;

int main(int argc, char *argv[]) {
    int id, numProcess = 0;
    
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    MPI_Comm_size(MPI_COMM_WORLD,&numProcess);
    
    if (id == 0) {
        HashMap* final_map;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        FileBuffer buffer = read_file_to_buffer("files/cr7.txt");
        FileBuffer* buffer_partitions = partition_buffer(buffer, numProcess);

        // printf("---------Buffer particionado---------\n");
        // for (int i = 0; i < numProcess; i++) {
        //     printf("Partição %d: ", i);
        //     for (int j = 0; j < buffer_partitions[i].size; j++) {
        //         printf("%c", buffer_partitions[i].data[j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
    
        for (int i = 1; i < numProcess; i++) {
            FileBuffer partition = buffer_partitions[i];
            MPI_Send(&partition.size, 1, MPI_UNSIGNED_LONG, i, PARTITION_SIZE, MPI_COMM_WORLD);
            MPI_Send(partition.data, partition.size, MPI_BYTE, i, PARTITION_PTR, MPI_COMM_WORLD);
        }

        FileBuffer master_partition = buffer_partitions[0];
        HashMap* partial_map = count_words(master_partition);
        print_hashmap(partial_map);

        // for (int i=1; i<=numProcess; i++) {
        //     HashMap* partial_map;
        //     MPI_Recv(partial_map, 3, MPI_INT, i, MAPS, MPI_COMM_WORLD, &status);
        //     merge_maps(&final_map, partial_map);
        //     free(partial_map);
        // }

        // free_file_buffer(buffer);
        // print_hashmap(final_map);
        // free_hashmap(final_map);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
    
        double elapsed_time = (end.tv_sec - start.tv_sec) + 
            (end.tv_nsec - start.tv_nsec) / 1e9;
        //printf("%.6f,%d\n", elapsed_time, TOTAL_THREADS);
        
    } else {
        FileBuffer buffer;
        ProcessArgs args; 
        
        MPI_Recv(&buffer.size, 1, MPI_UNSIGNED_LONG, 0, PARTITION_SIZE, MPI_COMM_WORLD, &status);

        char* data = malloc(buffer.size);
        MPI_Recv(data, buffer.size, MPI_BYTE, 0, PARTITION_PTR, MPI_COMM_WORLD, &status);
        buffer.data = data;

        // printf("Process %d: recieved buffer size: %ld\n", id, buffer.size);
        // printf("Process %d: recieved buffer data: %s\n", id, buffer.data);

        HashMap* partial_map = count_words(buffer);
        print_hashmap(partial_map);

        // MPI_Send(partial_map, 3, MPI_INT, 0, MAPS, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    return 0;
}