#include <stdio.h>
#include <stdlib.h>

struct Tuple {
    int *elements;
    int size;
};

struct Chunk {
    struct Tuple *tuples;
    int count;
};

struct ChunkResult {
    struct Chunk *chunks;
    int total_chunks;
};

struct ChunkResult chunk_tuples(const struct Tuple *tuples, int num_tuples, int n) {
    struct ChunkResult result;
    result.chunks = NULL;
    result.total_chunks = 0;

    if (tuples == NULL || num_tuples <= 0 || n <= 0) {
        return result;
    }

    int num_chunks = (num_tuples + n - 1) / n;
    result.chunks = (struct Chunk *)malloc(num_chunks * sizeof(struct Chunk));
    if (result.chunks == NULL) {
        return result;
    }
    result.total_chunks = num_chunks;

    for (int i = 0; i < num_chunks; i++) {
        int start = i * n;
        int remaining = num_tuples - start;
        int chunk_size = remaining < n ? remaining : n;

        result.chunks[i].tuples = (struct Tuple *)malloc(chunk_size * sizeof(struct Tuple));
        if (result.chunks[i].tuples == NULL) {
            for (int j = 0; j < i; j++) {
                free(result.chunks[j].tuples);
            }
            free(result.chunks);
            result.chunks = NULL;
            result.total_chunks = 0;
            return result;
        }
        result.chunks[i].count = chunk_size;

        for (int j = 0; j < chunk_size; j++) {
            result.chunks[i].tuples[j] = tuples[start + j];
        }
    }

    return result;
}

void free_chunk_result(struct ChunkResult result) {
    if (result.chunks != NULL) {
        for (int i = 0; i < result.total_chunks; i++) {
            free(result.chunks[i].tuples);
        }
        free(result.chunks);
    }
}

int main() {
    int num_tuples = 7;
    const int tuple_sizes[] = {3, 2, 1, 4, 2, 2, 3};
    struct Tuple *tuples = (struct Tuple *)malloc(num_tuples * sizeof(struct Tuple));
    if (tuples == NULL) {
        return 1;
    }

    for (int i = 0; i < num_tuples; i++) {
        tuples[i].size = tuple_sizes[i];
        tuples[i].elements = (int *)malloc(tuple_sizes[i] * sizeof(int));
        if (tuples[i].elements == NULL) {
            for (int j = 0; j < i; j++) {
                free(tuples[j].elements);
            }
            free(tuples);
            return 1;
        }
        for (int k = 0; k < tuple_sizes[i]; k++) {
            tuples[i].elements[k] = i * 10 + k;
        }
    }

    int n = 3;
    struct ChunkResult result = chunk_tuples(tuples, num_tuples, n);

    if (result.chunks != NULL) {
        for (int i = 0; i < result.total_chunks; i++) {
            printf("Chunk %d:\n", i);
            for (int j = 0; j < result.chunks[i].count; j++) {
                printf("  Tuple %d (size %d): ", j, result.chunks[i].tuples[j].size);
                for (int k = 0; k < result.chunks[i].tuples[j].size; k++) {
                    printf("%d ", result.chunks[i].tuples[j].elements[k]);
                }
                printf("\n");
            }
        }
    }

    free_chunk_result(result);

    for (int i = 0; i < num_tuples; i++) {
        free(tuples[i].elements);
    }
    free(tuples);

    return 0;
}