#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t count;
} Run;

static int decode_rle(const Run *runs, size_t run_count, int **decoded, size_t *decoded_count)
{
    size_t total = 0;
    size_t position = 0;
    int *result = NULL;

    if (decoded == NULL || decoded_count == NULL ||
        (runs == NULL && run_count != 0)) {
        return EINVAL;
    }

    *decoded = NULL;
    *decoded_count = 0;

    for (size_t i = 0; i < run_count; ++i) {
        if (runs[i].count > SIZE_MAX - total) {
            return EOVERFLOW;
        }
        total += runs[i].count;
    }

    if (total == 0) {
        return 0;
    }

    if (total > SIZE_MAX / sizeof(*result)) {
        return EOVERFLOW;
    }

    result = malloc(total * sizeof(*result));
    if (result == NULL) {
        return ENOMEM;
    }

    for (size_t i = 0; i < run_count; ++i) {
        for (size_t j = 0; j < runs[i].count; ++j) {
            result[position++] = runs[i].value;
        }
    }

    *decoded = result;
    *decoded_count = total;
    return 0;
}

int main(void)
{
    const Run encoded[] = {
        {1, 3},
        {4, 2},
        {7, 4}
    };
    int *decoded = NULL;
    size_t decoded_count = 0;
    int status = decode_rle(
        encoded,
        sizeof(encoded) / sizeof(encoded[0]),
        &decoded,
        &decoded_count
    );

    if (status != 0) {
        fprintf(stderr, "Decoding failed: error %d\n", status);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < decoded_count; ++i) {
        if (printf("%d%s", decoded[i], i + 1 == decoded_count ? "\n" : " ") < 0) {
            free(decoded);
            return EXIT_FAILURE;
        }
    }

    free(decoded);
    return EXIT_SUCCESS;
}