#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int interleave_lists(const int *first, const int *second, size_t length,
                            int **result, size_t *result_length)
{
    int *interleaved;
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
        length > SIZE_MAX / (2U * sizeof(*interleaved))) {
        return -1;
    }

    interleaved = malloc(2U * length * sizeof(*interleaved));
    if (interleaved == NULL) {
        return -1;
    }

    for (i = 0; i < length; ++i) {
        interleaved[2U * i] = first[i];
        interleaved[2U * i + 1U] = second[i];
    }

    *result = interleaved;
    *result_length = 2U * length;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4};
    const int second[] = {10, 20, 30, 40};
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
        if (printf("%d%s", result[i], i + 1U < result_length ? " " : "\n") < 0) {
            free(result);
            return EXIT_FAILURE;
        }
    }

    free(result);
    return EXIT_SUCCESS;
}