#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    const int64_t *elements;
    size_t length;
} IntList;

static int find_max_sum_list(const IntList *lists, size_t list_count,
                             size_t *result_index, int64_t *result_sum)
{
    if (lists == NULL || result_index == NULL || result_sum == NULL ||
        list_count == 0) {
        return -1;
    }

    size_t max_index = 0;
    int64_t max_sum = 0;

    for (size_t i = 0; i < list_count; ++i) {
        if (lists[i].elements == NULL && lists[i].length != 0) {
            return -1;
        }

        int64_t sum = 0;

        for (size_t j = 0; j < lists[i].length; ++j) {
            const int64_t value = lists[i].elements[j];

            if ((value > 0 && sum > INT64_MAX - value) ||
                (value < 0 && sum < INT64_MIN - value)) {
                return -1;
            }

            sum += value;
        }

        if (i == 0 || sum > max_sum) {
            max_sum = sum;
            max_index = i;
        }
    }

    *result_index = max_index;
    *result_sum = max_sum;
    return 0;
}

int main(void)
{
    const int64_t first[] = {1, 2, 3};
    const int64_t second[] = {10, -2, 5};
    const int64_t third[] = {4, 4, 4};

    const IntList lists[] = {
        {first, sizeof first / sizeof first[0]},
        {second, sizeof second / sizeof second[0]},
        {third, sizeof third / sizeof third[0]}
    };

    size_t max_index = 0;
    int64_t max_sum = 0;

    if (find_max_sum_list(lists, sizeof lists / sizeof lists[0],
                          &max_index, &max_sum) != 0) {
        fputs("Unable to determine the list with the highest sum.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("List %zu has the highest sum: %" PRId64 "\n",
           max_index, max_sum);

    return EXIT_SUCCESS;
}