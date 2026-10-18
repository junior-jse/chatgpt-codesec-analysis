#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    int first;
    int second;
    size_t frequency;
} Tuple;

static int assign_frequencies(Tuple *tuples, size_t count)
{
    size_t i;
    size_t j;

    if (count > 0 && tuples == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        tuples[i].frequency = 0;
    }

    for (i = 0; i < count; ++i) {
        if (tuples[i].frequency != 0) {
            continue;
        }

        size_t frequency = 1;

        for (j = i + 1; j < count; ++j) {
            if (tuples[i].first == tuples[j].first &&
                tuples[i].second == tuples[j].second) {
                ++frequency;
            }
        }

        for (j = i; j < count; ++j) {
            if (tuples[i].first == tuples[j].first &&
                tuples[i].second == tuples[j].second) {
                tuples[j].frequency = frequency;
            }
        }
    }

    return 0;
}

int main(void)
{
    Tuple tuples[] = {
        {1, 2, 0},
        {3, 4, 0},
        {1, 2, 0},
        {5, 6, 0},
        {3, 4, 0},
        {1, 2, 0}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    size_t i;

    if (assign_frequencies(tuples, count) != 0) {
        (void)fprintf(stderr, "Failed to assign tuple frequencies.\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("(%d, %d): %" PRIuMAX "\n",
                   tuples[i].first,
                   tuples[i].second,
                   (uintmax_t)tuples[i].frequency) < 0) {
            (void)fprintf(stderr, "Failed to write output.\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}