#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int interleave_lists(const int *first, const int *second, size_t length,
                            int **result, size_t *result_length)
{
    int *output;
    size_t i;

    if (result == NULL || result_length == NULL) {
        return -1;
    }

    *result = NULL;
    *result_length = 0;

    if (length == 0) {
        return 0;
    }

    if (first == NULL || second == NULL ||
        length > SIZE_MAX / (2 * sizeof(*output))) {
        return -1;
    }

    output = malloc(2 * length * sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    for (i = 0; i < length; ++i) {
        output[2 * i] = first[i];
        output[2 * i + 1] = second[i];
    }

    *result = output;
    *result_length = 2 * length;
    return 0;
}

int main(void)
{
    const int first[] = {1, 3, 5, 7};
    const int second[] = {2, 4, 6, 8};
    const size_t length = sizeof(first) / sizeof(first[0]);
    int *result = NULL;
    size_t result_length = 0;
    size_t i;

    if (length != sizeof(second) / sizeof(second[0])) {
        fputs("Lists must have the same length.\n", stderr);
        return EXIT_FAILURE;
    }

    if (interleave_lists(first, second, length, &result, &result_length) != 0) {
        fputs("Failed to interleave lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < result_length; ++i) {
        if (printf("%d%s", result[i], i + 1 < result_length ? " " : "\n") < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}