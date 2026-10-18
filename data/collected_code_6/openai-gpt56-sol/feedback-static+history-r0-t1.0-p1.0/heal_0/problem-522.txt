#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int print_longest_bitonic_subsequence(const int *array, size_t length)
{
    size_t *increasing = NULL;
    size_t *decreasing = NULL;
    size_t *previous = NULL;
    size_t *next = NULL;
    size_t *sequence = NULL;
    size_t best_index = 0;
    size_t best_length = 0;
    size_t position = 0;
    size_t index = 0;

    if (array == NULL || length == 0 || length > SIZE_MAX / sizeof(*increasing)) {
        return 0;
    }

    increasing = malloc(length * sizeof(*increasing));
    decreasing = malloc(length * sizeof(*decreasing));
    previous = malloc(length * sizeof(*previous));
    next = malloc(length * sizeof(*next));

    if (increasing == NULL || decreasing == NULL ||
        previous == NULL || next == NULL) {
        free(increasing);
        free(decreasing);
        free(previous);
        free(next);
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        increasing[i] = 1;
        previous[i] = SIZE_MAX;

        for (size_t j = 0; j < i; ++j) {
            if (array[j] < array[i] &&
                increasing[j] + 1 > increasing[i]) {
                increasing[i] = increasing[j] + 1;
                previous[i] = j;
            }
        }
    }

    for (size_t i = length; i-- > 0;) {
        decreasing[i] = 1;
        next[i] = SIZE_MAX;

        for (size_t j = i + 1; j < length; ++j) {
            if (array[j] < array[i] &&
                decreasing[j] + 1 > decreasing[i]) {
                decreasing[i] = decreasing[j] + 1;
                next[i] = j;
            }
        }

        if (increasing[i] + decreasing[i] - 1 > best_length) {
            best_length = increasing[i] + decreasing[i] - 1;
            best_index = i;
        }
    }

    if (best_length > SIZE_MAX / sizeof(*sequence)) {
        free(increasing);
        free(decreasing);
        free(previous);
        free(next);
        return 0;
    }

    sequence = malloc(best_length * sizeof(*sequence));
    if (sequence == NULL) {
        free(increasing);
        free(decreasing);
        free(previous);
        free(next);
        return 0;
    }

    position = increasing[best_index];
    index = best_index;

    while (index != SIZE_MAX) {
        sequence[--position] = index;
        index = previous[index];
    }

    position = increasing[best_index];
    index = next[best_index];

    while (index != SIZE_MAX) {
        sequence[position++] = index;
        index = next[index];
    }

    printf("Length: %zu\nSequence:", best_length);
    for (size_t i = 0; i < best_length; ++i) {
        printf(" %d", array[sequence[i]]);
    }
    putchar('\n');

    free(sequence);
    free(increasing);
    free(decreasing);
    free(previous);
    free(next);
    return 1;
}

int main(void)
{
    const int array[] = {1, 11, 2, 10, 4, 5, 2, 1};
    const size_t length = sizeof(array) / sizeof(array[0]);

    if (!print_longest_bitonic_subsequence(array, length)) {
        fputs("Unable to find the longest bitonic subsequence.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}