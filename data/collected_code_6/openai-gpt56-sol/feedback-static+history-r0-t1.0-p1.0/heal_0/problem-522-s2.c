#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *values;
    size_t length;
} IntSequence;

static void free_sequence(IntSequence *sequence)
{
    if (sequence != NULL) {
        free(sequence->values);
        sequence->values = NULL;
        sequence->length = 0;
    }
}

static int longest_bitonic_subsequence(const int *array, size_t length,
                                       IntSequence *result)
{
    size_t *increasing = NULL;
    size_t *decreasing = NULL;
    size_t *increasing_prev = NULL;
    size_t *decreasing_next = NULL;
    int *values = NULL;
    size_t peak = 0;
    size_t best_length = 0;
    size_t left_length;
    size_t position;
    size_t i;
    size_t j;

    if (result == NULL || (array == NULL && length != 0)) {
        return 0;
    }

    result->values = NULL;
    result->length = 0;

    if (length == 0) {
        return 1;
    }

    if (length > SIZE_MAX / sizeof(*increasing) ||
        length > SIZE_MAX / sizeof(*values)) {
        return 0;
    }

    increasing = malloc(length * sizeof(*increasing));
    decreasing = malloc(length * sizeof(*decreasing));
    increasing_prev = malloc(length * sizeof(*increasing_prev));
    decreasing_next = malloc(length * sizeof(*decreasing_next));

    if (increasing == NULL || decreasing == NULL ||
        increasing_prev == NULL || decreasing_next == NULL) {
        free(increasing);
        free(decreasing);
        free(increasing_prev);
        free(decreasing_next);
        return 0;
    }

    for (i = 0; i < length; ++i) {
        increasing[i] = 1;
        increasing_prev[i] = SIZE_MAX;

        for (j = 0; j < i; ++j) {
            if (array[j] < array[i] &&
                increasing[j] + 1 > increasing[i]) {
                increasing[i] = increasing[j] + 1;
                increasing_prev[i] = j;
            }
        }
    }

    for (i = length; i-- > 0;) {
        decreasing[i] = 1;
        decreasing_next[i] = SIZE_MAX;

        for (j = i + 1; j < length; ++j) {
            if (array[j] < array[i] &&
                decreasing[j] + 1 > decreasing[i]) {
                decreasing[i] = decreasing[j] + 1;
                decreasing_next[i] = j;
            }
        }

        if (increasing[i] + decreasing[i] - 1 > best_length) {
            best_length = increasing[i] + decreasing[i] - 1;
            peak = i;
        }
    }

    values = malloc(best_length * sizeof(*values));
    if (values == NULL) {
        free(increasing);
        free(decreasing);
        free(increasing_prev);
        free(decreasing_next);
        return 0;
    }

    left_length = increasing[peak];
    position = left_length;
    i = peak;

    while (i != SIZE_MAX) {
        values[--position] = array[i];
        i = increasing_prev[i];
    }

    position = left_length;
    i = decreasing_next[peak];

    while (i != SIZE_MAX) {
        values[position++] = array[i];
        i = decreasing_next[i];
    }

    free(increasing);
    free(decreasing);
    free(increasing_prev);
    free(decreasing_next);

    result->values = values;
    result->length = best_length;
    return 1;
}

int main(void)
{
    int array[] = {1, 11, 2, 10, 4, 5, 2, 1};
    size_t length = sizeof(array) / sizeof(array[0]);
    IntSequence result;
    size_t i;

    if (!longest_bitonic_subsequence(array, length, &result)) {
        fputs("Failed to find the longest bitonic subsequence.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Length: %zu\n", result.length);
    printf("Subsequence:");

    for (i = 0; i < result.length; ++i) {
        printf(" %d", result.values[i]);
    }

    putchar('\n');
    free_sequence(&result);
    return EXIT_SUCCESS;
}