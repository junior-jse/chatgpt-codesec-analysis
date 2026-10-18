#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

typedef struct {
    int value;
    size_t count;
} Run;

static int decode_rle(const Run *encoded, size_t run_count,
                      int **decoded, size_t *decoded_count)
{
    size_t total = 0;
    size_t position = 0;
    int *result = NULL;

    if (decoded == NULL || decoded_count == NULL ||
        (encoded == NULL && run_count != 0)) {
        return EINVAL;
    }

    *decoded = NULL;
    *decoded_count = 0;

    for (size_t i = 0; i < run_count; ++i) {
        if (encoded[i].count > SIZE_MAX - total) {
            return EOVERFLOW;
        }
        total += encoded[i].count;
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
        for (size_t j = 0; j < encoded[i].count; ++j) {
            result[position++] = encoded[i].value;
        }
    }

    *decoded = result;
    *decoded_count = total;
    return 0;
}

int main(void)
{
    const Run encoded[] = {
        {4, 3},
        {7, 2},
        {-1, 4},
        {9, 1}
    };
    const size_t run_count = sizeof(encoded) / sizeof(encoded[0]);
    int *decoded = NULL;
    size_t decoded_count = 0;
    int status = decode_rle(encoded, run_count, &decoded, &decoded_count);

    if (status != 0) {
        errno = status;
        perror("decode_rle");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < decoded_count; ++i) {
        if (printf("%s%d", i == 0 ? "" : " ", decoded[i]) < 0) {
            free(decoded);
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        free(decoded);
        return EXIT_FAILURE;
    }

    free(decoded);
    return EXIT_SUCCESS;
}