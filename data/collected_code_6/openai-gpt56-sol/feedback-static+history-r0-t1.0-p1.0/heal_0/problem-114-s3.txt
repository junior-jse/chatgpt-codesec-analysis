#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

typedef struct {
    int first;
    int second;
    size_t frequency;
} Tuple;

static int assign_frequencies(Tuple *tuples, size_t count)
{
    if (tuples == NULL && count != 0) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t frequency = 0;

        for (size_t j = 0; j < count; ++j) {
            if (tuples[i].first == tuples[j].first &&
                tuples[i].second == tuples[j].second) {
                if (frequency == SIZE_MAX) {
                    return -1;
                }
                ++frequency;
            }
        }

        tuples[i].frequency = frequency;
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

    if (assign_frequencies(tuples, count) != 0) {
        if (fputs("Failed to assign tuple frequencies.\n", stderr) == EOF) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (printf("(%d, %d): %zu\n",
                   tuples[i].first,
                   tuples[i].second,
                   tuples[i].frequency) < 0) {
            return EXIT_FAILURE;
        }
    }

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}