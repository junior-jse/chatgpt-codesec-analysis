#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
} ChunkResult;

ChunkResult chunk_tuples(Tuple *tuples, size_t tuple_count, size_t n) {
    ChunkResult result = {NULL, 0};

    if (tuples == NULL || tuple_count == 0 || n == 0) {
        return result;
    }

    result.count = (tuple_count + n - 1) / n;
    result.chunks = (Chunk *)malloc(result.count * sizeof(Chunk));
    if (result.chunks == NULL) {
        result.count = 0;
        return result;
    }

    for (size_t i = 0; i < result.count; i++) {
        size_t start = i * n;
        size_t remaining = tuple_count - start;
        result.chunks[i].count = remaining < n ? remaining : n;
        result.chunks[i].items = (Tuple *)malloc(result.chunks[i].count * sizeof(Tuple));
        
        if (result.chunks[i].items == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result.chunks[j].items);
            }
            free(result.chunks);
            result.chunks = NULL;
            result.count = 0;
            return result;
        }

        for (size_t k = 0; k < result.chunks[i].count; k++) {
            result.chunks[i].items[k] = tuples[start + k];
        }
    }

    return result;
}

void free_chunk_result(ChunkResult result) {
    for (size_t i = 0; i < result.count; i++) {
        free(result.chunks[i].items);
    }
    free(result.chunks);
}

int main() {
    Tuple tuples[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t n = 2;

    ChunkResult result = chunk_tuples(tuples, tuple_count, n);

    if (result.chunks == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result.count; i++) {
        printf("Chunk %zu: ", i);
        for (size_t j = 0; j < result.chunks[i].count; j++) {
            printf("(%d, %d) ", result.chunks[i].items[j].a, result.chunks[i].items[j].b);
        }
        printf("\n");
    }

    free_chunk_result(result);

    return EXIT_SUCCESS;
}