#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

static bool find_max_sum(const int64_t *const *lists,
                         const size_t *lengths,
                         size_t list_count,
                         int64_t *max_sum)
{
    if (lists == NULL || lengths == NULL || max_sum == NULL || list_count == 0U) {
        return false;
    }

    bool found = false;
    int64_t maximum = 0;

    for (size_t i = 0U; i < list_count; ++i) {
        if (lengths[i] > 0U && lists[i] == NULL) {
            return false;
        }

        int64_t sum = 0;

        for (size_t j = 0U; j < lengths[i]; ++j) {
            const int64_t value = lists[i][j];

            if ((value > 0 && sum > INT64_MAX - value) ||
                (value < 0 && sum < INT64_MIN - value)) {
                return false;
            }

            sum += value;
        }

        if (!found || sum > maximum) {
            maximum = sum;
            found = true;
        }
    }

    *max_sum = maximum;
    return true;
}

int main(void)
{
    static const int64_t list1[] = {1, 2, 3};
    static const int64_t list2[] = {-5, 10, 4};
    static const int64_t list3[] = {7, 1};

    const int64_t *lists[] = {list1, list2, list3};
    const size_t lengths[] = {
        sizeof list1 / sizeof list1[0],
        sizeof list2 / sizeof list2[0],
        sizeof list3 / sizeof list3[0]
    };

    int64_t maximum = 0;

    if (!find_max_sum(lists, lengths, sizeof lists / sizeof lists[0], &maximum)) {
        fputs("Unable to calculate the maximum sum.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%" PRId64 "\n", maximum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}