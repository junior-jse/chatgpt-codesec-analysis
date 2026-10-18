#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t count;
} Run;

static int run_length_encode(const int *input, size_t input_length,
                             Run **output, size_t *output_length)
{
    Run *encoded;
    size_t run_count;
    size_t i;

    if (output == NULL || output_length == NULL) {
        return -1;
    }

    *output = NULL;
    *output_length = 0;

    if (input_length == 0) {
        return 0;
    }

    if (input == NULL || input_length > SIZE_MAX / sizeof(*encoded)) {
        return -1;
    }

    encoded = malloc(input_length * sizeof(*encoded));
    if (encoded == NULL) {
        return -1;
    }

    run_count = 0;
    encoded[run_count].value = input[0];
    encoded[run_count].count = 1;

    for (i = 1; i < input_length; ++i) {
        if (input[i] == encoded[run_count].value) {
            ++encoded[run_count].count;
        } else {
            ++run_count;
            encoded[run_count].value = input[i];
            encoded[run_count].count = 1;
        }
    }

    ++run_count;
    *output = encoded;
    *output_length = run_count;
    return 0;
}

int main(void)
{
    const int input[] = {1, 1, 1, 2, 2, 3, 4, 4, 5};
    const size_t input_length = sizeof(input) / sizeof(input[0]);
    Run *encoded = NULL;
    size_t encoded_length = 0;
    size_t i;

    if (run_length_encode(input, input_length, &encoded, &encoded_length) != 0) {
        fputs("Run-length encoding failed.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < encoded_length; ++i) {
        if (printf("(%d, %zu)%s",
                   encoded[i].value,
                   encoded[i].count,
                   i + 1 == encoded_length ? "\n" : " ") < 0) {
            free(encoded);
            return EXIT_FAILURE;
        }
    }

    free(encoded);
    return EXIT_SUCCESS;
}