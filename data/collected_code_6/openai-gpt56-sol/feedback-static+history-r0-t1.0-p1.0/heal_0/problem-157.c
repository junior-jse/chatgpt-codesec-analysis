#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t count;
} Run;

int run_length_encode(const int *input, size_t length, Run **output, size_t *output_length)
{
    Run *runs;
    size_t run_count = 0;

    if (output == NULL || output_length == NULL) {
        return -1;
    }

    *output = NULL;
    *output_length = 0;

    if (length == 0) {
        return 0;
    }

    if (input == NULL || length > SIZE_MAX / sizeof(*runs)) {
        return -1;
    }

    runs = malloc(length * sizeof(*runs));
    if (runs == NULL) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        if (run_count == 0 || runs[run_count - 1].value != input[i]) {
            runs[run_count].value = input[i];
            runs[run_count].count = 1;
            ++run_count;
        } else {
            ++runs[run_count - 1].count;
        }
    }

    *output = runs;
    *output_length = run_count;
    return 0;
}

int main(void)
{
    const int values[] = {1, 1, 1, 2, 2, 3, 1, 1, 4};
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    Run *encoded = NULL;
    size_t encoded_count = 0;

    if (run_length_encode(values, value_count, &encoded, &encoded_count) != 0) {
        fputs("Run-length encoding failed.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < encoded_count; ++i) {
        if (printf("(%d, %zu)%s",
                   encoded[i].value,
                   encoded[i].count,
                   i + 1 < encoded_count ? " " : "\n") < 0) {
            free(encoded);
            return EXIT_FAILURE;
        }
    }

    free(encoded);
    return EXIT_SUCCESS;
}