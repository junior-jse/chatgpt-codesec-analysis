#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple tuple;
    int frequency;
} TupleFrequency;

int compare_tuples(const void *x, const void *y) {
    const Tuple *tx = (const Tuple *)x;
    const Tuple *ty = (const Tuple *)y;
    if (tx->a != ty->a) {
        return tx->a - ty->a;
    }
    return tx->b - ty->b;
}

int assign_frequencies(Tuple *tuples, size_t tuple_count, TupleFrequency **result, size_t *result_count) {
    if (tuple_count == 0) {
        *result = NULL;
        *result_count = 0;
        return 0;
    }

    Tuple *sorted = (Tuple *)malloc(tuple_count * sizeof(Tuple));
    if (sorted == NULL) {
        return -1;
    }
    memcpy(sorted, tuples, tuple_count * sizeof(Tuple));
    qsort(sorted, tuple_count, sizeof(Tuple), compare_tuples);

    size_t unique_count = 1;
    for (size_t i = 1; i < tuple_count; ++i) {
        if (compare_tuples(&sorted[i], &sorted[i - 1]) != 0) {
            unique_count++;
        }
    }

    TupleFrequency *freqs = (TupleFrequency *)malloc(unique_count * sizeof(TupleFrequency));
    if (freqs == NULL) {
        free(sorted);
        return -1;
    }

    size_t idx = 0;
    freqs[idx].tuple = sorted[0];
    freqs[idx].frequency = 1;

    for (size_t i = 1; i < tuple_count; ++i) {
        if (compare_tuples(&sorted[i], &sorted[i - 1]) == 0) {
            freqs[idx].frequency++;
        } else {
            idx++;
            freqs[idx].tuple = sorted[i];
            freqs[idx].frequency = 1;
        }
    }

    free(sorted);
    *result = freqs;
    *result_count = unique_count;
    return 0;
}

int main(void) {
    Tuple data[] = {{1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}};
    size_t data_count = sizeof(data) / sizeof(data[0]);

    TupleFrequency *frequencies = NULL;
    size_t freq_count = 0;

    if (assign_frequencies(data, data_count, &frequencies, &freq_count) != 0) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < freq_count; ++i) {
        printf("(%d, %d): %d\n", frequencies[i].tuple.a, frequencies[i].tuple.b, frequencies[i].frequency);
    }

    free(frequencies);
    return EXIT_SUCCESS;
}