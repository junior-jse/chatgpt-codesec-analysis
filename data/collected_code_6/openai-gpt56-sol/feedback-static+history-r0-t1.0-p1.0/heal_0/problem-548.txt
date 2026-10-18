#include <stdio.h>
#include <stdlib.h>

static size_t lower_bound(const int *array, size_t length, int value)
{
    size_t left = 0;
    size_t right = length;

    while (left < right) {
        size_t middle = left + (right - left) / 2;

        if (array[middle] < value) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

static int longest_increasing_subsequence(const int *sequence,
                                          size_t length,
                                          size_t *result)
{
    int *tails;
    size_t lis_length = 0;
    size_t i;

    if (result == NULL || (sequence == NULL && length != 0)) {
        return -1;
    }

    *result = 0;

    if (length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*tails)) {
        return -1;
    }

    tails = malloc(length * sizeof(*tails));
    if (tails == NULL) {
        return -1;
    }

    for (i = 0; i < length; ++i) {
        size_t position = lower_bound(tails, lis_length, sequence[i]);

        tails[position] = sequence[i];
        if (position == lis_length) {
            ++lis_length;
        }
    }

    free(tails);
    *result = lis_length;
    return 0;
}

int main(void)
{
    size_t length;
    size_t lis_length;
    int *sequence = NULL;
    size_t i;

    if (scanf("%zu", &length) != 1) {
        fputs("Invalid sequence length\n", stderr);
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(*sequence)) {
        fputs("Sequence is too large\n", stderr);
        return EXIT_FAILURE;
    }

    if (length != 0) {
        sequence = malloc(length * sizeof(*sequence));
        if (sequence == NULL) {
            fputs("Memory allocation failed\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < length; ++i) {
        if (scanf("%d", &sequence[i]) != 1) {
            fputs("Invalid sequence element\n", stderr);
            free(sequence);
            return EXIT_FAILURE;
        }
    }

    if (longest_increasing_subsequence(sequence, length, &lis_length) != 0) {
        fputs("Unable to compute the result\n", stderr);
        free(sequence);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", lis_length) < 0) {
        fputs("Output error\n", stderr);
        free(sequence);
        return EXIT_FAILURE;
    }

    free(sequence);
    return EXIT_SUCCESS;
}