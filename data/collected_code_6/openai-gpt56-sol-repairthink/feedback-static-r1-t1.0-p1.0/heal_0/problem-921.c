#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const int *data;
    size_t length;
} IntTuple;

typedef struct {
    const IntTuple *tuples;
    size_t count;
} TupleChunk;

static int chunk_tuples(const IntTuple *tuples, size_t tuple_count,
                        size_t tuple_size, size_t chunk_size,
                        TupleChunk **chunks_out, size_t *chunk_count_out)
{
    TupleChunk *chunks;
    size_t chunk_count;
    size_t i;

    if (chunks_out == NULL || chunk_count_out == NULL ||
        chunk_size == 0U || (tuple_count != 0U && tuples == NULL)) {
        return EINVAL;
    }

    *chunks_out = NULL;
    *chunk_count_out = 0U;

    for (i = 0U; i < tuple_count; ++i) {
        if (tuples[i].length != tuple_size ||
            (tuple_size != 0U && tuples[i].data == NULL)) {
            return EINVAL;
        }
    }

    if (tuple_count == 0U) {
        return 0;
    }

    chunk_count = tuple_count / chunk_size;
    if (tuple_count % chunk_size != 0U) {
        ++chunk_count;
    }

    if (chunk_count > SIZE_MAX / sizeof(*chunks)) {
        return EOVERFLOW;
    }

    chunks = calloc(chunk_count, sizeof(*chunks));
    if (chunks == NULL) {
        return ENOMEM;
    }

    for (i = 0U; i < chunk_count; ++i) {
        size_t offset = i * chunk_size;
        size_t remaining = tuple_count - offset;

        chunks[i].tuples = &tuples[offset];
        chunks[i].count = remaining < chunk_size ? remaining : chunk_size;
    }

    *chunks_out = chunks;
    *chunk_count_out = chunk_count;
    return 0;
}

int main(void)
{
    static const int tuple_data[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12},
        {13, 14, 15}
    };
    IntTuple tuples[sizeof(tuple_data) / sizeof(tuple_data[0])];
    TupleChunk *chunks = NULL;
    size_t chunk_count = 0U;
    size_t i;
    size_t j;
    size_t k;
    int result;

    for (i = 0U; i < sizeof(tuples) / sizeof(tuples[0]); ++i) {
        tuples[i].data = tuple_data[i];
        tuples[i].length = sizeof(tuple_data[i]) / sizeof(tuple_data[i][0]);
    }

    result = chunk_tuples(
        tuples,
        sizeof(tuples) / sizeof(tuples[0]),
        sizeof(tuple_data[0]) / sizeof(tuple_data[0][0]),
        2U,
        &chunks,
        &chunk_count
    );

    if (result != 0) {
        fprintf(stderr, "chunk_tuples failed: %d\n", result);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < chunk_count; ++i) {
        printf("Chunk %zu:\n", i);
        for (j = 0U; j < chunks[i].count; ++j) {
            for (k = 0U; k < chunks[i].tuples[j].length; ++k) {
                printf("%d%s",
                       chunks[i].tuples[j].data[k],
                       k + 1U == chunks[i].tuples[j].length ? "\n" : " ");
            }
        }
    }

    free(chunks);
    return EXIT_SUCCESS;
}