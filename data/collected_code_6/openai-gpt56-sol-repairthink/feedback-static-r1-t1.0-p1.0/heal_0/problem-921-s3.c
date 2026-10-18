#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} Chunk;

static void free_chunks(Chunk *chunks, size_t count)
{
    if (chunks == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(chunks[i].items);
    }

    free(chunks);
}

static int chunk_tuples(const int *tuples, size_t tuple_count, size_t tuple_size,
                        size_t tuples_per_chunk, Chunk **result,
                        size_t *chunk_count)
{
    if (result == NULL || chunk_count == NULL || tuple_size == 0 ||
        tuples_per_chunk == 0 || (tuples == NULL && tuple_count != 0)) {
        return EINVAL;
    }

    *result = NULL;
    *chunk_count = 0;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX - (tuples_per_chunk - 1)) {
        return EOVERFLOW;
    }

    size_t count = (tuple_count + tuples_per_chunk - 1) / tuples_per_chunk;

    if (count > SIZE_MAX / sizeof(Chunk)) {
        return EOVERFLOW;
    }

    Chunk *chunks = calloc(count, sizeof(*chunks));
    if (chunks == NULL) {
        return ENOMEM;
    }

    size_t offset = 0;

    for (size_t i = 0; i < count; ++i) {
        size_t remaining = tuple_count - offset;
        size_t current_tuples =
            remaining < tuples_per_chunk ? remaining : tuples_per_chunk;

        if (current_tuples > SIZE_MAX / tuple_size) {
            free_chunks(chunks, count);
            return EOVERFLOW;
        }

        size_t item_count = current_tuples * tuple_size;

        if (item_count > SIZE_MAX / sizeof(*chunks[i].items)) {
            free_chunks(chunks, count);
            return EOVERFLOW;
        }

        chunks[i].items = malloc(item_count * sizeof(*chunks[i].items));
        if (chunks[i].items == NULL) {
            free_chunks(chunks, count);
            return ENOMEM;
        }

        chunks[i].length = item_count;

        if (offset > SIZE_MAX / tuple_size) {
            free_chunks(chunks, count);
            return EOVERFLOW;
        }

        size_t source_index = offset * tuple_size;

        for (size_t j = 0; j < item_count; ++j) {
            chunks[i].items[j] = tuples[source_index + j];
        }

        offset += current_tuples;
    }

    *result = chunks;
    *chunk_count = count;
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
    Chunk *chunks = NULL;
    size_t chunk_count = 0;

    int status = chunk_tuples(&tuples[0][0], tuple_count, tuple_size,
                              tuples_per_chunk, &chunks, &chunk_count);
    if (status != 0) {
        errno = status;
        perror("chunk_tuples");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < chunk_count; ++i) {
        if (printf("Chunk %zu:", i + 1) < 0) {
            free_chunks(chunks, chunk_count);
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < chunks[i].length; ++j) {
            if (printf(" %d", chunks[i].items[j]) < 0) {
                free_chunks(chunks, chunk_count);
                return EXIT_FAILURE;
            }
        }

        if (putchar('\n') == EOF) {
            free_chunks(chunks, chunk_count);
            return EXIT_FAILURE;
        }
    }

    free_chunks(chunks, chunk_count);

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}