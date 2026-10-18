#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    const int64_t *elements;
    size_t length;
} IntList;

typedef enum {
    FIND_MAX_OK,
    FIND_MAX_INVALID_ARGUMENT,
    FIND_MAX_OVERFLOW,
    FIND_MAX_EMPTY
} FindMaxResult;

static int add_int64_checked(int64_t a, int64_t b, int64_t *result)
{
    if ((b > 0 && a > INT64_MAX - b) ||
        (b < 0 && a < INT64_MIN - b)) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static FindMaxResult find_list_with_highest_sum(
    const IntList *lists,
    size_t list_count,
    size_t *max_index,
    int64_t *max_sum)
{
    size_t i;
    int found = 0;

    if (lists == NULL || max_index == NULL || max_sum == NULL) {
        return FIND_MAX_INVALID_ARGUMENT;
    }

    if (list_count == 0) {
        return FIND_MAX_EMPTY;
    }

    for (i = 0; i < list_count; ++i) {
        size_t j;
        int64_t sum = 0;

        if (lists[i].length > 0 && lists[i].elements == NULL) {
            return FIND_MAX_INVALID_ARGUMENT;
        }

        for (j = 0; j < lists[i].length; ++j) {
            if (!add_int64_checked(sum, lists[i].elements[j], &sum)) {
                return FIND_MAX_OVERFLOW;
            }
        }

        if (!found || sum > *max_sum) {
            *max_sum = sum;
            *max_index = i;
            found = 1;
        }
    }

    return FIND_MAX_OK;
}

int main(void)
{
    static const int64_t first[] = {4, 7, -2};
    static const int64_t second[] = {10, 3, 8};
    static const int64_t third[] = {20, -1, -5};
    static const IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    size_t max_index = 0;
    int64_t max_sum = 0;
    FindMaxResult result;
    size_t i;

    result = find_list_with_highest_sum(
        lists,
        sizeof(lists) / sizeof(lists[0]),
        &max_index,
        &max_sum
    );

    if (result != FIND_MAX_OK) {
        fprintf(stderr, "Unable to find the list with the highest sum: %d\n",
                (int)result);
        return EXIT_FAILURE;
    }

    printf("List %zu:", max_index);

    for (i = 0; i < lists[max_index].length; ++i) {
        /* Possible weaknesses found:
         * Flawfinder printf: If format strings can be influenced by an attacker, they can be exploited (CWE-134). Use a constant for the format specification. (risk 4, format)
         */
        printf(" %" PRId64, lists[max_index].elements[i]);
    }

    printf("\nSum: %" PRId64 "\n", max_sum);

    return EXIT_SUCCESS;
}