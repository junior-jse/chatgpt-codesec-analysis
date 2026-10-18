#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

typedef struct {
    const int64_t *elements;
    size_t length;
} IntList;

static int find_max_list_sum(const IntList *lists, size_t list_count, int64_t *maximum)
{
    int64_t max_sum = 0;

    if (lists == NULL || maximum == NULL || list_count == 0) {
        return -1;
    }

    for (size_t i = 0; i < list_count; ++i) {
        int64_t sum = 0;

        if (lists[i].elements == NULL && lists[i].length != 0) {
            return -1;
        }

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
        }
    }

    *maximum = max_sum;
    return 0;
}

int main(void)
{
    const int64_t first[] = {1, 2, 3};
    const int64_t second[] = {-4, 10, 2};
    const int64_t third[] = {7, 1};

    const IntList lists[] = {
        {first, sizeof first / sizeof first[0]},
        {second, sizeof second / sizeof second[0]},
        {third, sizeof third / sizeof third[0]}
    };

    int64_t maximum;

    if (find_max_list_sum(lists, sizeof lists / sizeof lists[0], &maximum) != 0) {
        errno = ERANGE;
        perror("Unable to calculate the maximum list sum");
        return EXIT_FAILURE;
    }

    if (printf("%" PRId64 "\n", maximum) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}