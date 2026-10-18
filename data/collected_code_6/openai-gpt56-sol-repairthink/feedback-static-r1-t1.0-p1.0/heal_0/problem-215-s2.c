#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t count;
} Run;

int decode_rle(const Run *encoded, size_t encoded_length,
               int **decoded, size_t *decoded_length)
{
    size_t total = 0;
    size_t position = 0;
    int *result = NULL;

    if (decoded == NULL || decoded_length == NULL ||
        (encoded == NULL && encoded_length != 0)) {
        return EINVAL;
    }

    *decoded = NULL;
    *decoded_length = 0;

    for (size_t i = 0; i < encoded_length; ++i) {
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

    for (size_t i = 0; i < encoded_length; ++i) {
        for (size_t j = 0; j < encoded[i].count; ++j) {
            result[position++] = encoded[i].value;
        }
    }

    *decoded = result;
    *decoded_length = total;
    return 0;
}

int main(void)
{
    const Run encoded[] = {
        {1, 3},
        {5, 2},
        {8, 4}
    };
    int *decoded = NULL;
    size_t decoded_length = 0;
    int status = decode_rle(encoded,
                            sizeof(encoded) / sizeof(encoded[0]),
                            &decoded,
                            &decoded_length);

    if (status != 0) {
        errno = status;
        perror("decode_rle");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < decoded_length; ++i) {
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