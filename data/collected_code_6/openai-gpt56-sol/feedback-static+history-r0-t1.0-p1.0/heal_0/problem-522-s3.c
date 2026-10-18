#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static int find_longest_bitonic_subsequence(
    const int *array,
    size_t length,
    int **subsequence,
    size_t *subsequence_length
)
{
    size_t *increasing = NULL;
    size_t *decreasing = NULL;
    size_t *predecessor = NULL;
    size_t *successor = NULL;
    int *result = NULL;
    size_t peak = 0U;
    size_t best_length = 0U;

    if (array == NULL || subsequence == NULL || subsequence_length == NULL) {
        return -1;
    }

    *subsequence = NULL;
    *subsequence_length = 0U;

    if (length == 0U) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*increasing) ||
        length > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    increasing = malloc(length * sizeof(*increasing));
    decreasing = malloc(length * sizeof(*decreasing));
    predecessor = malloc(length * sizeof(*predecessor));
    successor = malloc(length * sizeof(*successor));

    if (increasing == NULL || decreasing == NULL ||
        predecessor == NULL || successor == NULL) {
        free(increasing);
        free(decreasing);
        free(predecessor);
        free(successor);
        return -1;
    }

    for (size_t i = 0U; i < length; ++i) {
        increasing[i] = 1U;
        predecessor[i] = SIZE_MAX;

        for (size_t j = 0U; j < i; ++j) {
            if (array[j] < array[i] &&
                increasing[j] + 1U > increasing[i]) {
                increasing[i] = increasing[j] + 1U;
                predecessor[i] = j;
            }
        }
    }

    for (size_t i = length; i-- > 0U;) {
        decreasing[i] = 1U;
        successor[i] = SIZE_MAX;

        for (size_t j = i + 1U; j < length; ++j) {
            if (array[j] < array[i] &&
                decreasing[j] + 1U > decreasing[i]) {
                decreasing[i] = decreasing[j] + 1U;
                successor[i] = j;
            }
        }

        if (increasing[i] + decreasing[i] - 1U > best_length) {
            best_length = increasing[i] + decreasing[i] - 1U;
            peak = i;
        }
    }

    result = malloc(best_length * sizeof(*result));
    if (result == NULL) {
        free(increasing);
        free(decreasing);
        free(predecessor);
        free(successor);
        return -1;
    }

    size_t position = increasing[peak];
    size_t index = peak;

    while (index != SIZE_MAX) {
        result[--position] = array[index];
        index = predecessor[index];
    }

    position = increasing[peak];
    index = successor[peak];

    while (index != SIZE_MAX) {
        result[position++] = array[index];
        index = successor[index];
    }

    free(increasing);
    free(decreasing);
    free(predecessor);
    free(successor);

    *subsequence = result;
    *subsequence_length = best_length;
    return 0;
}

int main(void)
{
    int array[] = {1, 11, 2, 10, 4, 5, 2, 1};
    const size_t length = sizeof(array) / sizeof(array[0]);
    int *subsequence = NULL;
    size_t subsequence_length = 0U;

    if (find_longest_bitonic_subsequence(
            array,
            length,
            &subsequence,
            &subsequence_length
        ) != 0) {
        fputs("Failed to find the longest bitonic subsequence.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Length: %zu\nSubsequence:", subsequence_length);
    for (size_t i = 0U; i < subsequence_length; ++i) {
        printf(" %d", subsequence[i]);
    }
    putchar('\n');

    free(subsequence);
    return EXIT_SUCCESS;
}