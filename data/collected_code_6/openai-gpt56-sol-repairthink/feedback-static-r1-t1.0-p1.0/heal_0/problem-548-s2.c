#include <stdio.h>
#include <stdlib.h>

static size_t longest_increasing_subsequence(const int *sequence, size_t length)
{
    int *tails;
    size_t lis_length = 0;

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

    for (size_t i = 0; i < length; ++i) {
        size_t left = 0;
        size_t right = lis_length;

        while (left < right) {
            size_t middle = left + (right - left) / 2;

            if (tails[middle] < sequence[i]) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }

        tails[left] = sequence[i];

        if (left == lis_length) {
            ++lis_length;
        }
    }

    free(tails);
    return lis_length;
}

int main(void)
{
    size_t length;

    if (scanf("%zu", &length) != 1) {
        fprintf(stderr, "Failed to read sequence length.\n");
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Sequence is too large.\n");
        return EXIT_FAILURE;
    }

    int *sequence = NULL;

    if (length > 0) {
        sequence = malloc(length * sizeof(*sequence));
        if (sequence == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < length; ++i) {
        if (scanf("%d", &sequence[i]) != 1) {
            fprintf(stderr, "Failed to read sequence element.\n");
            free(sequence);
            return EXIT_FAILURE;
        }
    }

    size_t result = longest_increasing_subsequence(sequence, length);

    if (length > 0 && result == 0) {
        fprintf(stderr, "Unable to compute the result.\n");
        free(sequence);
        return EXIT_FAILURE;
    }

    printf("%zu\n", result);
    free(sequence);

    return EXIT_SUCCESS;
}