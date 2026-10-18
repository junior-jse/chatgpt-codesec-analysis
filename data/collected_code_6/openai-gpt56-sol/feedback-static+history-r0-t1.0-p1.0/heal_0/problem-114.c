#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int first;
    int second;
    size_t frequency;
} Tuple;

static int tuple_compare(const void *lhs, const void *rhs)
{
    const Tuple *a = lhs;
    const Tuple *b = rhs;

    if (a->first < b->first) {
        return -1;
    }
    if (a->first > b->first) {
        return 1;
    }
    if (a->second < b->second) {
        return -1;
    }
    if (a->second > b->second) {
        return 1;
    }
    return 0;
}

static int assign_frequencies(Tuple *tuples, size_t count)
{
    size_t start;

    if (tuples == NULL && count != 0U) {
        return -1;
    }

    if (count == 0U) {
        return 0;
    }

    qsort(tuples, count, sizeof(*tuples), tuple_compare);

    start = 0U;
    while (start < count) {
        size_t end = start + 1U;
        size_t frequency;

        while (end < count &&
               tuples[end].first == tuples[start].first &&
               tuples[end].second == tuples[start].second) {
            ++end;
        }

        frequency = end - start;
        for (size_t i = start; i < end; ++i) {
            tuples[i].frequency = frequency;
        }

        start = end;
    }

    return 0;
}

int main(void)
{
    Tuple tuples[] = {
        {1, 2, 0U},
        {3, 4, 0U},
        {1, 2, 0U},
        {5, 6, 0U},
        {3, 4, 0U},
        {1, 2, 0U}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);

    if (assign_frequencies(tuples, count) != 0) {
        fputs("Failed to assign tuple frequencies.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("(%d, %d): %zu\n",
                   tuples[i].first,
                   tuples[i].second,
                   tuples[i].frequency) < 0) {
            fputs("Failed to write output.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}