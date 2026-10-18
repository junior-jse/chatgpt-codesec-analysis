#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t tuple_size;
    size_t num_tuples;
} TupleSet;

typedef struct {
    TupleSet *chunks;
    size_t num_chunks;
} ChunkResult;

int chunk_tuples(const int *source, size_t total_tuples, size_t tuple_size, 
                 size_t chunk_size, ChunkResult *result) {
    size_t i;
    size_t num_chunks;
    
    if (source == NULL || result == NULL || tuple_size == 0 || chunk_size == 0) {
        return -1;
    }
    
    if (total_tuples == 0) {
        result->chunks = NULL;
        result->num_chunks = 0;
        return 0;
    }
    
    num_chunks = (total_tuples + chunk_size - 1) / chunk_size;
    
    result->chunks = (TupleSet *)calloc(num_chunks, sizeof(TupleSet));
    if (result->chunks == NULL) {
        return -1;
    }
    result->num_chunks = num_chunks;
    
    for (i = 0; i < num_chunks; i++) {
        size_t current_chunk_size = (i == num_chunks - 1) ? 
                            (total_tuples - i * chunk_size) : chunk_size;
        
        result->chunks[i].data = (int *)malloc(current_chunk_size * tuple_size * sizeof(int));
        if (result->chunks[i].data == NULL) {
            size_t k;
            for (k = 0; k < i; k++) {
                free(result->chunks[k].data);
            }
            free(result->chunks);
            result->chunks = NULL;
            result->num_chunks = 0;
            return -1;
        }
        
        result->chunks[i].tuple_size = tuple_size;
        result->chunks[i].num_tuples = current_chunk_size;
        
        memcpy(result->chunks[i].data, 
               source + (i * chunk_size * tuple_size),
               current_chunk_size * tuple_size * sizeof(int));
    }
    
    return 0;
}

void free_chunk_result(ChunkResult *result) {
    if (result == NULL) {
        return;
    }
    
    if (result->chunks != NULL) {
        size_t i;
        for (i = 0; i < result->num_chunks; i++) {
            free(result->chunks[i].data);
        }
        free(result->chunks);
        result->chunks = NULL;
    }
    result->num_chunks = 0;
}

int main(void) {
    const int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    size_t tuple_size = 3;
    size_t total_tuples = 5;
    size_t chunk_size = 2;
    ChunkResult result;
    size_t i;
    size_t j;
    size_t k;
    
    if (chunk_tuples(data, total_tuples, tuple_size, chunk_size, &result) != 0) {
        fprintf(stderr, "Error: Failed to chunk tuples\n");
        return EXIT_FAILURE;
    }
    
    printf("Original tuples (size %zu):\n", tuple_size);
    for (i = 0; i < total_tuples; i++) {
        printf("(");
        for (j = 0; j < tuple_size; j++) {
            printf("%d", data[i * tuple_size + j]);
            if (j < tuple_size - 1) {
                printf(", ");
            }
        }
        printf(")");
        if (i < total_tuples - 1) {
            printf(" ");
        }
    }
    printf("\n\n");
    
    printf("Chunked into %zu chunks (chunk size %zu):\n", result.num_chunks, chunk_size);
    for (i = 0; i < result.num_chunks; i++) {
        printf("Chunk %zu: ", i);
        for (j = 0; j < result.chunks[i].num_tuples; j++) {
            printf("(");
            for (k = 0; k < result.chunks[i].tuple_size; k++) {
                printf("%d", result.chunks[i].data[j * result.chunks[i].tuple_size + k]);
                if (k < result.chunks[i].tuple_size - 1) {
                    printf(", ");
                }
            }
            printf(")");
            if (j < result.chunks[i].num_tuples - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    free_chunk_result(&result);
    
    return EXIT_SUCCESS;
}