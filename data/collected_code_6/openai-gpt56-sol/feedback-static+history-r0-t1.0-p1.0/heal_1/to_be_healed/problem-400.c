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

static int compare_tuples(const void *left, const void *right)
{
    const Tuple *a = left;
    const Tuple *b = right;

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

static int get_tuple_frequencies(const Tuple *tuples, size_t count,
                                 TupleFrequency **frequencies,
                                 size_t *unique_count)
{
    Tuple *sorted = NULL;
    TupleFrequency *result = NULL;
    size_t result_count = 0;
    size_t i;

    if (frequencies == NULL || unique_count == NULL) {
        return -1;
    }

    *frequencies = NULL;
    *unique_count = 0;

    if (count == 0) {
        return 0;
    }

    if (tuples == NULL ||
        count > SIZE_MAX / sizeof(*sorted) ||
        count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    sorted = malloc(count * sizeof(*sorted));
    result = malloc(count * sizeof(*result));

    if (sorted == NULL || result == NULL) {
        free(sorted);
        free(result);
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(sorted, tuples, count * sizeof(*sorted));
    qsort(sorted, count, sizeof(*sorted), compare_tuples);

    for (i = 0; i < count; ++i) {
        if (result_count == 0 ||
            compare_tuples(&sorted[i], &result[result_count - 1].tuple) != 0) {
            result[result_count].tuple = sorted[i];
            result[result_count].frequency = 1;
            ++result_count;
        } else {
            ++result[result_count - 1].frequency;
        }
    }

    free(sorted);

    if (result_count < count) {
        TupleFrequency *resized =
            realloc(result, result_count * sizeof(*result));

        if (resized != NULL) {
            result = resized;
        }
    }

    *frequencies = result;
    *unique_count = result_count;
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
    size_t i;

    if (get_tuple_frequencies(tuples, count, &frequencies,
                              &unique_count) != 0) {
        fputs("Failed to extract tuple frequencies.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < unique_count; ++i) {
        printf("(%d, %d): %zu\n",
               frequencies[i].tuple.first,
               frequencies[i].tuple.second,
               frequencies[i].frequency);
    }

    free(frequencies);
    return EXIT_SUCCESS;
}