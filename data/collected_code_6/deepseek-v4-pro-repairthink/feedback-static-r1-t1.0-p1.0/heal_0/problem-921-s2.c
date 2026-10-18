#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **chunks;
    int *chunk_sizes;
    int num_chunks;
    int tuple_size;
} TupleChunker;

TupleChunker *create_chunker(const int *tuples, int num_tuples, int tuple_size, int chunk_size) {
    if (tuples == NULL || tuple_size <= 0 || chunk_size <= 0 || num_tuples < 0) {
        return NULL;
    }
    
    TupleChunker *chunker = (TupleChunker *)malloc(sizeof(TupleChunker));
    if (chunker == NULL) {
        return NULL;
    }
    
    chunker->num_chunks = 0;
    chunker->tuple_size = tuple_size;
    chunker->chunk_sizes = NULL;
    chunker->chunks = NULL;
    
    if (num_tuples == 0) {
        chunker->chunk_sizes = (int *)malloc(sizeof(int));
        if (chunker->chunk_sizes == NULL) {
            free(chunker);
            return NULL;
        }
        chunker->chunk_sizes[0] = 0;
        chunker->chunks = (int **)malloc(sizeof(int *));
        if (chunker->chunks == NULL) {
            free(chunker->chunk_sizes);
            free(chunker);
            return NULL;
        }
        chunker->chunks[0] = NULL;
        chunker->num_chunks = 0;
        return chunker;
    }
    
    chunker->num_chunks = (num_tuples + chunk_size - 1) / chunk_size;
    
    chunker->chunk_sizes = (int *)malloc(sizeof(int) * chunker->num_chunks);
    if (chunker->chunk_sizes == NULL) {
        free(chunker);
        return NULL;
    }
    
    chunker->chunks = (int **)malloc(sizeof(int *) * chunker->num_chunks);
    if (chunker->chunks == NULL) {
        free(chunker->chunk_sizes);
        free(chunker);
        return NULL;
    }
    
    for (int i = 0; i < chunker->num_chunks; i++) {
        int start = i * chunk_size;
        int end = start + chunk_size;
        if (end > num_tuples) {
            end = num_tuples;
        }
        chunker->chunk_sizes[i] = end - start;
        chunker->chunks[i] = (int *)malloc(sizeof(int) * chunker->chunk_sizes[i] * tuple_size);
        if (chunker->chunks[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(chunker->chunks[j]);
            }
            free(chunker->chunks);
            free(chunker->chunk_sizes);
            free(chunker);
            return NULL;
        }
        
        for (int j = start; j < end; j++) {
            for (int k = 0; k < tuple_size; k++) {
                chunker->chunks[i][(j - start) * tuple_size + k] = tuples[j * tuple_size + k];
            }
        }
    }
    
    return chunker;
}

void destroy_chunker(TupleChunker *chunker) {
    if (chunker == NULL) {
        return;
    }
    
    if (chunker->chunks != NULL) {
        for (int i = 0; i < chunker->num_chunks; i++) {
            free(chunker->chunks[i]);
        }
        free(chunker->chunks);
    }
    
    free(chunker->chunk_sizes);
    free(chunker);
}

void print_chunks(const TupleChunker *chunker) {
    if (chunker == NULL) {
        printf("Invalid chunker\n");
        return;
    }
    
    for (int i = 0; i < chunker->num_chunks; i++) {
        printf("Chunk %d (size %d): ", i, chunker->chunk_sizes[i]);
        for (int j = 0; j < chunker->chunk_sizes[i]; j++) {
            printf("(");
            for (int k = 0; k < chunker->tuple_size; k++) {
                printf("%d", chunker->chunks[i][j * chunker->tuple_size + k]);
                if (k < chunker->tuple_size - 1) {
                    printf(",");
                }
            }
            printf(")");
            if (j < chunker->chunk_sizes[i] - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(void) {
    int tuples[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };
    int num_tuples = 5;
    int tuple_size = 3;
    int chunk_size = 2;
    
    TupleChunker *chunker = create_chunker(tuples, num_tuples, tuple_size, chunk_size);
    if (chunker == NULL) {
        fprintf(stderr, "Failed to create chunker\n");
        return 1;
    }
    
    print_chunks(chunker);
    destroy_chunker(chunker);
    
    return 0;
}