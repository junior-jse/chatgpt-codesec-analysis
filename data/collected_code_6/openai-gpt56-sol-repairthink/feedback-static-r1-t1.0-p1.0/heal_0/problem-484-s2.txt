#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int first;
    int second;
} Tuple;

static bool tuples_equal(const Tuple *left, const Tuple *right)
{
    return left->first == right->first && left->second == right->second;
}

static bool contains_tuple(const Tuple *tuples, size_t count, const Tuple *target)
{
    for (size_t i = 0; i < count; ++i) {
        if (tuples_equal(&tuples[i], target)) {
            return true;
        }
    }

    return false;
}

static size_t remove_matching_tuples(
    Tuple *first,
    size_t first_count,
    Tuple *second,
    size_t second_count
)
{
    if ((first == NULL && first_count != 0U) ||
        (second == NULL && second_count != 0U)) {
        return 0U;
    }

    size_t write_index = 0U;

    for (size_t i = 0U; i < first_count; ++i) {
        if (!contains_tuple(second, second_count, &first[i])) {
            first[write_index++] = first[i];
        }
    }

    return write_index;
}

static int print_tuples(const Tuple *tuples, size_t count)
{
    if (tuples == NULL && count != 0U) {
        return -1;
    }

    if (printf("(") < 0) {
        return -1;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("%s(%d, %d)",
                   i == 0U ? "" : ", ",
                   tuples[i].first,
                   tuples[i].second) < 0) {
            return -1;
        }
    }

    return printf(")\n") < 0 ? -1 : 0;
}

int main(void)
{
    Tuple first[] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };
    Tuple second[] = {
        {3, 4},
        {7, 8},
        {9, 10}
    };

    const size_t first_count = sizeof first / sizeof first[0];
    const size_t second_count = sizeof second / sizeof second[0];

    size_t remaining_count = remove_matching_tuples(
        first,
        first_count,
        second,
        second_count
    );

    if (print_tuples(first, remaining_count) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}