#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const int *elements;
    size_t length;
} IntList;

static int find_max_sum(const IntList *lists, size_t list_count, long long *result)
{
    long long maximum = LLONG_MIN;
    size_t i;

    if (lists == NULL || result == NULL || list_count == 0U) {
        return 0;
    }

    for (i = 0U; i < list_count; ++i) {
        long long sum = 0;
        size_t j;

        if (lists[i].elements == NULL && lists[i].length != 0U) {
            return 0;
        }

        for (j = 0U; j < lists[i].length; ++j) {
            int value = lists[i].elements[j];

            if ((value > 0 && sum > LLONG_MAX - value) ||
                (value < 0 && sum < LLONG_MIN - value)) {
                return 0;
            }

            sum += value;
        }

        if (sum > maximum) {
            maximum = sum;
        }
    }

    *result = maximum;
    return 1;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {-5, 10, 4};
    const int third[] = {7, -2, 1};
    const IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    long long maximum;

    if (!find_max_sum(lists, sizeof(lists) / sizeof(lists[0]), &maximum)) {
        fputs("Failed to calculate the maximum sum.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%lld\n", maximum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}