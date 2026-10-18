#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int max_sum_bitonic_subsequence(const int *array, size_t length, long long *result)
{
    long long *increasing = NULL;
    long long *decreasing = NULL;
    long long maximum;
    size_t i;
    size_t j;

    if (array == NULL || result == NULL || length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*increasing)) {
        return 0;
    }

    increasing = malloc(length * sizeof(*increasing));
    decreasing = malloc(length * sizeof(*decreasing));

    if (increasing == NULL || decreasing == NULL) {
        free(increasing);
        free(decreasing);
        return 0;
    }

    for (i = 0; i < length; ++i) {
        increasing[i] = array[i];

        for (j = 0; j < i; ++j) {
            if (array[j] < array[i] &&
                increasing[i] < increasing[j] + array[i]) {
                increasing[i] = increasing[j] + array[i];
            }
        }
    }

    for (i = length; i-- > 0;) {
        decreasing[i] = array[i];

        for (j = i + 1; j < length; ++j) {
            if (array[j] < array[i] &&
                decreasing[i] < decreasing[j] + array[i]) {
                decreasing[i] = decreasing[j] + array[i];
            }
        }
    }

    maximum = increasing[0] + decreasing[0] - array[0];

    for (i = 1; i < length; ++i) {
        long long sum = increasing[i] + decreasing[i] - array[i];

        if (sum > maximum) {
            maximum = sum;
        }
    }

    *result = maximum;

    free(increasing);
    free(decreasing);
    return 1;
}

int main(void)
{
    int array[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    const size_t length = sizeof(array) / sizeof(array[0]);
    long long result;

    if (!max_sum_bitonic_subsequence(array, length, &result)) {
        fputs("Unable to calculate the maximum sum.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%lld\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}