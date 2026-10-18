#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int a;
    int b;
} Tuple;

typedef struct TupleFreq {
    int a;
    int b;
    int freq;
} TupleFreq;

int tuple_compare(const void *x, const void *y) {
    const Tuple *t1 = (const Tuple *)x;
    const Tuple *t2 = (const Tuple *)y;
    if (t1->a != t2->a)
        return t1->a - t2->a;
    return t1->b - t2->b;
}

int tuple_freq_compare(const void *x, const void *y) {
    const TupleFreq *t1 = (const TupleFreq *)x;
    const TupleFreq *t2 = (const TupleFreq *)y;
    if (t1->a != t2->a)
        return t1->a - t2->a;
    return t1->b - t2->b;
}

TupleFreq *assign_frequency(Tuple *tuples, int size, int *result_size) {
    if (tuples == NULL || size <= 0 || result_size == NULL) {
        *result_size = 0;
        return NULL;
    }

    Tuple *sorted = (Tuple *)malloc(sizeof(Tuple) * size);
    if (sorted == NULL) {
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        sorted[i] = tuples[i];
    }

    qsort(sorted, size, sizeof(Tuple), tuple_compare);

    TupleFreq *result = (TupleFreq *)malloc(sizeof(TupleFreq) * size);
    if (result == NULL) {
        free(sorted);
        *result_size = 0;
        return NULL;
    }

    int count = 0;
    int i = 0;

    while (i < size) {
        int j = i;
        while (j < size && sorted[j].a == sorted[i].a && sorted[j].b == sorted[i].b) {
            j++;
        }
        result[count].a = sorted[i].a;
        result[count].b = sorted[i].b;
        result[count].freq = j - i;
        count++;
        i = j;
    }

    TupleFreq *final_result = (TupleFreq *)realloc(result, sizeof(TupleFreq) * count);
    if (final_result == NULL && count > 0) {
        free(sorted);
        free(result);
        *result_size = 0;
        return NULL;
    }

    if (count == 0) {
        free(sorted);
        free(result);
        *result_size = 0;
        return NULL;
    }

    qsort(final_result, count, sizeof(TupleFreq), tuple_freq_compare);

    free(sorted);
    *result_size = count;
    return final_result;
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {1, 2},
        {2, 3},
        {3, 4},
        {1, 2},
        {5, 6}
    };

    int size = sizeof(tuples) / sizeof(tuples[0]);
    int result_size = 0;

    TupleFreq *result = assign_frequency(tuples, size, &result_size);

    if (result == NULL) {
        printf("Memory allocation failed or invalid input\n");
        return 1;
    }

    printf("Tuple Frequencies:\n");
    for (int i = 0; i < result_size; i++) {
        printf("(%d, %d): %d\n", result[i].a, result[i].b, result[i].freq);
    }

    free(result);
    return 0;
}