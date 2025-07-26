#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>  
#include "definitions.h"
#include "utils/hash/fnv_hash.h"
#include "utils/hashmap/hashmap.h"
#include "utils/io/read_file.h"
#include "utils/count/count.h"
#include <mpi.h>

typedef enum {
    PARTITION_SIZE = 1,
    PARTITION_PTR = 2,
    TOTAL_ENTRIES = 3,
    PARTIAL_MAP = 4,
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
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        FileBuffer buffer = read_file_to_buffer(FILE_PATH);
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
        HashMap* master_map = count_words(master_partition);

        for (int i = 1; i < numProcess; i++) {
            int total_entries;
            MPI_Recv(&total_entries, 1, MPI_INT, i, TOTAL_ENTRIES, MPI_COMM_WORLD, &status);

            FlatEntry* flat_entries = malloc(total_entries * sizeof(FlatEntry));
            MPI_Recv(flat_entries, total_entries * sizeof(FlatEntry), MPI_BYTE, i, PARTIAL_MAP, MPI_COMM_WORLD, &status);
            
            for (int i = 0; i< total_entries; i++)
                add_with_count(&master_map, flat_entries[i].key, flat_entries[i].count);

            free(flat_entries);
        }

        print_hashmap(master_map);
        
        free_file_buffer(buffer);
        free(buffer_partitions);
        free_hashmap(master_map);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
    
        double elapsed_time = (end.tv_sec - start.tv_sec) + 
            (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("%.6f,%d\n", elapsed_time, TOTAL_THREADS);
        
    } else {
        FileBuffer buffer;
        
        MPI_Recv(&buffer.size, 1, MPI_UNSIGNED_LONG, 0, PARTITION_SIZE, MPI_COMM_WORLD, &status);

        buffer.data = malloc(buffer.size);
        MPI_Recv(buffer.data, buffer.size, MPI_BYTE, 0, PARTITION_PTR, MPI_COMM_WORLD, &status);

        // printf("Process %d: recieved buffer size: %ld\n", id, buffer.size);
        // printf("Process %d: recieved buffer data: %s\n", id, buffer.data);

        HashMap* partial_map = count_words(buffer);
        
        int total_entries = partial_map->n_items;
        FlatEntry* flat_entries = serialize_map(partial_map);

        MPI_Send(&total_entries, 1, MPI_INT, 0, TOTAL_ENTRIES, MPI_COMM_WORLD);
        MPI_Send(flat_entries, total_entries * sizeof(FlatEntry), MPI_BYTE, 0, PARTIAL_MAP, MPI_COMM_WORLD);

        free_hashmap(partial_map);
        free(flat_entries);
    }

    MPI_Finalize();
    
    return 0;
}