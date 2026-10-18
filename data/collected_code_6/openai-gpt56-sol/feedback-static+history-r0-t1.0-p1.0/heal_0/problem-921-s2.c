#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t length;
} TupleChunk;

static void free_chunks(TupleChunk *chunks, size_t count)
{
    if (chunks == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(chunks[i].values);
    }

    free(chunks);
}

static int chunk_tuples(const int *values, size_t tuple_count, size_t tuple_size,
                        size_t tuples_per_chunk, TupleChunk **result,
                        size_t *result_count)
{
    if (result == NULL || result_count == NULL || tuple_size == 0 ||
        tuples_per_chunk == 0 || (tuple_count != 0 && values == NULL)) {
        return EINVAL;
    }

    *result = NULL;
    *result_count = 0;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX - (tuples_per_chunk - 1)) {
        return EOVERFLOW;
    }

    size_t chunk_count =
        (tuple_count + tuples_per_chunk - 1) / tuples_per_chunk;

    if (chunk_count > SIZE_MAX / sizeof(TupleChunk)) {
        return EOVERFLOW;
    }

    TupleChunk *chunks = calloc(chunk_count, sizeof(*chunks));
    if (chunks == NULL) {
        return ENOMEM;
    }

    size_t tuple_offset = 0;

    for (size_t i = 0; i < chunk_count; ++i) {
        size_t remaining = tuple_count - tuple_offset;
        size_t tuples_in_chunk =
            remaining < tuples_per_chunk ? remaining : tuples_per_chunk;

        if (tuples_in_chunk > SIZE_MAX / tuple_size) {
            free_chunks(chunks, chunk_count);
            return EOVERFLOW;
        }

        size_t element_count = tuples_in_chunk * tuple_size;

        if (element_count > SIZE_MAX / sizeof(*chunks[i].values)) {
            free_chunks(chunks, chunk_count);
            return EOVERFLOW;
        }

        if (tuple_offset > SIZE_MAX / tuple_size) {
            free_chunks(chunks, chunk_count);
            return EOVERFLOW;
        }

        chunks[i].values = malloc(element_count * sizeof(*chunks[i].values));
        if (chunks[i].values == NULL) {
            free_chunks(chunks, chunk_count);
            return ENOMEM;
        }

        chunks[i].length = tuples_in_chunk;

        size_t source_offset = tuple_offset * tuple_size;

        for (size_t j = 0; j < element_count; ++j) {
            chunks[i].values[j] = values[source_offset + j];
        }

        tuple_offset += tuples_in_chunk;
    }

    *result = chunks;
    *result_count = chunk_count;
    return 0;
}

int main(void)
{
    const int tuples[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12},
        {13, 14, 15}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t tuple_size = sizeof(tuples[0]) / sizeof(tuples[0][0]);
    const size_t tuples_per_chunk = 2;

    TupleChunk *chunks = NULL;
    size_t chunk_count = 0;

    int status = chunk_tuples(&tuples[0][0], tuple_count, tuple_size,
                              tuples_per_chunk, &chunks, &chunk_count);
    if (status != 0) {
        fprintf(stderr, "chunk_tuples failed: %d\n", status);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < chunk_count; ++i) {
        printf("Chunk %zu:\n", i);

        for (size_t j = 0; j < chunks[i].length; ++j) {
            printf("(");

            for (size_t k = 0; k < tuple_size; ++k) {
                if (k != 0) {
                    printf(", ");
                }

                printf("%d", chunks[i].values[j * tuple_size + k]);
            }

            printf(")\n");
        }
    }

    free_chunks(chunks, chunk_count);
    return EXIT_SUCCESS;
}