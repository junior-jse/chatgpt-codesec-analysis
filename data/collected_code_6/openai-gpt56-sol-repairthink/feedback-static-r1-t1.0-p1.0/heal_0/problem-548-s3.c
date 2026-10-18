#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static size_t lower_bound(const int *values, size_t length, int target)
{
    size_t left = 0;
    size_t right = length;

    while (left < right) {
        size_t middle = left + (right - left) / 2;

        if (values[middle] < target) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

static size_t longest_increasing_subsequence(const int *sequence, size_t length)
{
    int *tails;
    size_t lis_length = 0;
    size_t i;

    if (sequence == NULL || length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*tails)) {
        return 0;
    }

    tails = malloc(length * sizeof(*tails));
    if (tails == NULL) {
        return 0;
    }

    for (i = 0; i < length; ++i) {
        size_t position = lower_bound(tails, lis_length, sequence[i]);

        tails[position] = sequence[i];
        if (position == lis_length) {
            ++lis_length;
        }
    }

    free(tails);
    return lis_length;
}

int main(void)
{
    size_t length;
    int *sequence;
    size_t result;
    size_t i;

    if (scanf("%zu", &length) != 1) {
        return EXIT_FAILURE;
    }

    if (length == 0) {
        printf("0\n");
        return EXIT_SUCCESS;
    }

    if (length > SIZE_MAX / sizeof(*sequence)) {
        return EXIT_FAILURE;
    }

    sequence = malloc(length * sizeof(*sequence));
    if (sequence == NULL) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < length; ++i) {
        if (scanf("%d", &sequence[i]) != 1) {
            free(sequence);
            return EXIT_FAILURE;
        }
    }

    result = longest_increasing_subsequence(sequence, length);
    free(sequence);

    if (result == 0) {
        return EXIT_FAILURE;
    }

    printf("%zu\n", result);
    return EXIT_SUCCESS;
}