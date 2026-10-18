#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple tuple;
    size_t frequency;
} TupleFrequency;

static int compare_tuples(const void *lhs, const void *rhs)
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

static int tuple_equal(Tuple a, Tuple b)
{
    return a.first == b.first && a.second == b.second;
}

static int tuple_frequencies(const Tuple *tuples, size_t count,
                             TupleFrequency **result, size_t *unique_count)
{
    Tuple *sorted = NULL;
    TupleFrequency *frequencies = NULL;
    size_t used = 0;

    if (result == NULL || unique_count == NULL ||
        (count > 0 && tuples == NULL)) {
        return -1;
    }

    *result = NULL;
    *unique_count = 0;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*sorted) ||
        count > SIZE_MAX / sizeof(*frequencies)) {
        return -1;
    }

    sorted = malloc(count * sizeof(*sorted));
    if (sorted == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(sorted, tuples, count * sizeof(*sorted));
    qsort(sorted, count, sizeof(*sorted), compare_tuples);

    frequencies = calloc(count, sizeof(*frequencies));
    if (frequencies == NULL) {
        free(sorted);
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (used == 0 || !tuple_equal(frequencies[used - 1].tuple, sorted[i])) {
            frequencies[used].tuple = sorted[i];
            frequencies[used].frequency = 1;
            ++used;
        } else {
            ++frequencies[used - 1].frequency;
        }
    }

    free(sorted);

    if (used < count) {
        TupleFrequency *resized = realloc(frequencies,
                                          used * sizeof(*frequencies));
        if (resized != NULL) {
            frequencies = resized;
        }
    }

    *result = frequencies;
    *unique_count = used;
    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {1, 2},
        {5, 6},
        {3, 4},
        {1, 2}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    TupleFrequency *frequencies = NULL;
    size_t unique_count = 0;

    if (tuple_frequencies(tuples, count, &frequencies, &unique_count) != 0) {
        fputs("Failed to calculate tuple frequencies.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < unique_count; ++i) {
        if (printf("(%d, %d): %zu\n",
                   frequencies[i].tuple.first,
                   frequencies[i].tuple.second,
                   frequencies[i].frequency) < 0) {
            free(frequencies);
            return EXIT_FAILURE;
        }
    }

    free(frequencies);
    return EXIT_SUCCESS;
}