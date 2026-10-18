#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static int max_sum_bitonic_subsequence(const int *array, size_t length, int64_t *result)
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
            int64_t candidate = increasing[j] + (int64_t)array[i];

            if (array[j] < array[i] && candidate > increasing[i]) {
                increasing[i] = candidate;
            }
        }
    }

    for (size_t i = length; i-- > 0;) {
        decreasing[i] = array[i];

        for (size_t j = i + 1; j < length; ++j) {
            int64_t candidate = decreasing[j] + (int64_t)array[i];

            if (array[j] < array[i] && candidate > decreasing[i]) {
                decreasing[i] = candidate;
            }
        }
    }

    maximum = increasing[0] + decreasing[0] - (int64_t)array[0];

    for (size_t i = 1; i < length; ++i) {
        int64_t sum = increasing[i] + decreasing[i] - (int64_t)array[i];

        if (sum > maximum) {
            maximum = sum;
        }
    }

    free(increasing);
    free(decreasing);
    *result = maximum;
    return 0;
}

int main(void)
{
    int array[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    size_t length = sizeof(array) / sizeof(array[0]);
    int64_t result;

    if (max_sum_bitonic_subsequence(array, length, &result) != 0) {
        fputs("Unable to calculate the maximum sum.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("%" PRId64 "\n", result);
    return EXIT_SUCCESS;
}