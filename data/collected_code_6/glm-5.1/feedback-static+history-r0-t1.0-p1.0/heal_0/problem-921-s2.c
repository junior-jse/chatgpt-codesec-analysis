#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple* data;
    size_t size;
} TupleChunk;

typedef struct {
    TupleChunk* chunks;
    size_t chunk_count;
} ChunkedTuples;

ChunkedTuples chunk_tuples(const Tuple* tuples, size_t total_tuples, size_t n) {
    ChunkedTuples result;
    result.chunks = NULL;
    result.chunk_count = 0;

    if (tuples == NULL || n == 0 || total_tuples == 0) {
        return result;
    }

    size_t num_chunks = (total_tuples + n - 1) / n;
    result.chunks = (TupleChunk*)malloc(num_chunks * sizeof(TupleChunk));
    if (result.chunks == NULL) {
        return result;
    }

    result.chunk_count = num_chunks;

    for (size_t i = 0; i < num_chunks; ++i) {
        size_t start = i * n;
        size_t chunk_size = n;
        if (start + chunk_size > total_tuples) {
            chunk_size = total_tuples - start;
        }

        result.chunks[i].data = (Tuple*)malloc(chunk_size * sizeof(Tuple));
        if (result.chunks[i].data == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(result.chunks[j].data);
            }
            free(result.chunks);
            result.chunks = NULL;
            result.chunk_count = 0;
            return result;
        }

        for (size_t k = 0; k < chunk_size; ++k) {
            result.chunks[i].data[k] = tuples[start + k];
        }
        result.chunks[i].size = chunk_size;
    }

    return result;
}

void free_chunked_tuples(ChunkedTuples* chunked) {
    if (chunked == NULL) {
        return;
    }
    if (chunked->chunks != NULL) {
        for (size_t i = 0; i < chunked->chunk_count; ++i) {
            free(chunked->chunks[i].data);
        }
        free(chunked->chunks);
        chunked->chunks = NULL;
    }
    chunked->chunk_count = 0;
}

int main(void) {
    Tuple tuples[] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};
    size_t total_tuples = sizeof(tuples) / sizeof(tuples[0]);
    size_t n = 2;

    ChunkedTuples chunked = chunk_tuples(tuples, total_tuples, n);

    for (size_t i = 0; i < chunked.chunk_count; ++i) {
        for (size_t j = 0; j < chunked.chunks[i].size; ++j) {
            printf("(%d, %d) ", chunked.chunks[i].data[j].a, chunked.chunks[i].data[j].b);
        }
        printf("\n");
    }

    free_chunked_tuples(&chunked);

    return 0;
}