#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int *values;
    size_t length;
    size_t frequency;
} TupleFrequency;

static int compare_ints(const void *lhs, const void *rhs)
{
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;
    return (a > b) - (a < b);
}

static int compare_tuples(const int *lhs, const int *rhs, size_t length)
{
    size_t i;

    for (i = 0; i < length; ++i) {
        if (lhs[i] < rhs[i]) {
            return -1;
        }
        if (lhs[i] > rhs[i]) {
            return 1;
        }
    }

    return 0;
}

static void free_tuple_frequencies(TupleFrequency *frequencies, size_t count)
{
    size_t i;

    if (frequencies == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free(frequencies[i].values);
    }

    free(frequencies);
}

static int extract_tuple_frequencies(
    const int *tuples,
    size_t tuple_count,
    size_t tuple_length,
    TupleFrequency **result,
    size_t *result_count)
{
    TupleFrequency *frequencies;
    int *normalized;
    size_t unique_count = 0;
    size_t i;
    size_t j;

    if (result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuples == NULL || tuple_length == 0 ||
        tuple_count > SIZE_MAX / sizeof(*frequencies) ||
        tuple_length > SIZE_MAX / sizeof(*normalized)) {
        return -1;
    }

    frequencies = calloc(tuple_count, sizeof(*frequencies));
    normalized = malloc(tuple_length * sizeof(*normalized));

    if (frequencies == NULL || normalized == NULL) {
        free(frequencies);
        free(normalized);
        return -1;
    }

    for (i = 0; i < tuple_count; ++i) {
        if (i > SIZE_MAX / tuple_length) {
            free(normalized);
            free_tuple_frequencies(frequencies, unique_count);
            return -1;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(
            normalized,
            tuples + i * tuple_length,
            tuple_length * sizeof(*normalized)
        );
        qsort(normalized, tuple_length, sizeof(*normalized), compare_ints);

        for (j = 0; j < unique_count; ++j) {
            if (compare_tuples(
                    frequencies[j].values,
                    normalized,
                    tuple_length) == 0) {
                if (frequencies[j].frequency == SIZE_MAX) {
                    free(normalized);
                    free_tuple_frequencies(frequencies, unique_count);
                    return -1;
                }

                ++frequencies[j].frequency;
                break;
            }
        }

        if (j == unique_count) {
            frequencies[unique_count].values =
                malloc(tuple_length * sizeof(*normalized));

            if (frequencies[unique_count].values == NULL) {
                free(normalized);
                free_tuple_frequencies(frequencies, unique_count);
                return -1;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(
                frequencies[unique_count].values,
                normalized,
                tuple_length * sizeof(*normalized)
            );
            frequencies[unique_count].length = tuple_length;
            frequencies[unique_count].frequency = 1;
            ++unique_count;
        }
    }

    free(normalized);
    *result = frequencies;
    *result_count = unique_count;
    return 0;
}

int main(void)
{
    const int tuples[][3] = {
        {1, 2, 3},
        {3, 1, 2},
        {4, 5, 6},
        {6, 4, 5},
        {1, 1, 2}
    };
    TupleFrequency *frequencies = NULL;
    size_t frequency_count = 0;
    size_t i;
    size_t j;

    if (extract_tuple_frequencies(
            &tuples[0][0],
            sizeof(tuples) / sizeof(tuples[0]),
            sizeof(tuples[0]) / sizeof(tuples[0][0]),
            &frequencies,
            &frequency_count) != 0) {
        fputs("Failed to extract tuple frequencies.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < frequency_count; ++i) {
        putchar('(');

        for (j = 0; j < frequencies[i].length; ++j) {
            if (j != 0) {
                fputs(", ", stdout);
            }
            printf("%d", frequencies[i].values[j]);
        }

        printf("): %zu\n", frequencies[i].frequency);
    }

    free_tuple_frequencies(frequencies, frequency_count);
    return EXIT_SUCCESS;
}