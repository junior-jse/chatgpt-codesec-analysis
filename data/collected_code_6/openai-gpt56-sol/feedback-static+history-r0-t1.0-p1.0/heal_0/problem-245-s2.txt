#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static int maximum_sum_bitonic_subsequence(const int *array, size_t length,
                                           int64_t *result)
{
    int64_t *increasing = NULL;
    int64_t *decreasing = NULL;
    int64_t maximum;

    if (array == NULL || result == NULL || length == 0 ||
        length > SIZE_MAX / sizeof(*increasing)) {
        return -1;
    }

    increasing = malloc(length * sizeof(*increasing));
    decreasing = malloc(length * sizeof(*decreasing));

    if (increasing == NULL || decreasing == NULL) {
        free(increasing);
        free(decreasing);
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        increasing[i] = array[i];

        for (size_t j = 0; j < i; ++j) {
            if (array[j] < array[i]) {
                int64_t candidate = increasing[j] + (int64_t)array[i];

                if (candidate > increasing[i]) {
                    increasing[i] = candidate;
                }
            }
        }
    }

    for (size_t i = length; i-- > 0;) {
        decreasing[i] = array[i];

        for (size_t j = i + 1; j < length; ++j) {
            if (array[j] < array[i]) {
                int64_t candidate = decreasing[j] + (int64_t)array[i];

                if (candidate > decreasing[i]) {
                    decreasing[i] = candidate;
                }
            }
        }
    }

    maximum = increasing[0] + decreasing[0] - (int64_t)array[0];

    for (size_t i = 1; i < length; ++i) {
        int64_t candidate =
            increasing[i] + decreasing[i] - (int64_t)array[i];

        if (candidate > maximum) {
            maximum = candidate;
        }
    }

    *result = maximum;
    free(increasing);
    free(decreasing);
    return 0;
}

int main(void)
{
    size_t length;
    int *array = NULL;
    int64_t result;

    if (scanf("%zu", &length) != 1 || length == 0 ||
        length > SIZE_MAX / sizeof(*array)) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    array = malloc(length * sizeof(*array));
    if (array == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            fputs("Invalid input\n", stderr);
            free(array);
            return EXIT_FAILURE;
        }
    }

    if (maximum_sum_bitonic_subsequence(array, length, &result) != 0) {
        fputs("Unable to compute result\n", stderr);
        free(array);
        return EXIT_FAILURE;
    }

    printf("%" PRId64 "\n", result);
    free(array);
    return EXIT_SUCCESS;
}