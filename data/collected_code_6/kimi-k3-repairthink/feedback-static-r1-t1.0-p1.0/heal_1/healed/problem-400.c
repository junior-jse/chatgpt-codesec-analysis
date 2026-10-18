#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple tuple;
    size_t count;
} TupleFreq;

static int compare_ints(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static int tuples_equal(const Tuple *a, const Tuple *b)
{
    if (a->size != b->size) {
        return 0;
    }
    if (a->size == 0) {
        return 1;
    }
    if (a->elements == NULL || b->elements == NULL) {
        return 0;
    }
    for (size_t i = 0; i < a->size; i++) {
        if (a->elements[i] != b->elements[i]) {
            return 0;
        }
    }
    return 1;
}

static void normalize_tuple(Tuple *t)
{
    if (t->elements != NULL && t->size > 1) {
        qsort(t->elements, t->size, sizeof(int), compare_ints);
    }
}

static int checked_int_bytes(size_t count, size_t *bytes)
{
    if (bytes == NULL || count > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    *bytes = count * sizeof(int);
    return 0;
}

static int copy_int_elements(int *dst, size_t dst_capacity, const int *src, size_t src_count)
{
    size_t bytes;

    if (src_count > dst_capacity) {
        return -1;
    }
    if (src_count == 0) {
        return 0;
    }
    if (dst == NULL || src == NULL) {
        return -1;
    }
    if (checked_int_bytes(src_count, &bytes) != 0) {
        return -1;
    }
    memcpy(dst, src, bytes);
    return 0;
}

static int duplicate_tuple(const Tuple *src, Tuple *dst)
{
    size_t bytes;

    if (src == NULL || dst == NULL) {
        return -1;
    }

    dst->size = 0;
    dst->elements = NULL;

    if (src->size == 0) {
        return 0;
    }
    if (src->elements == NULL) {
        return -1;
    }
    if (checked_int_bytes(src->size, &bytes) != 0) {
        return -1;
    }

    dst->elements = malloc(bytes);
    if (dst->elements == NULL) {
        return -1;
    }

    if (copy_int_elements(dst->elements, src->size, src->elements, src->size) != 0) {
        free(dst->elements);
        dst->elements = NULL;
        return -1;
    }

    dst->size = src->size;
    return 0;
}

TupleFreq *get_tuple_frequencies(Tuple *tuples, size_t num_tuples, size_t *result_size)
{
    Tuple *normalized;
    TupleFreq *result;
    size_t unique_count = 0;

    if (tuples == NULL || result_size == NULL || num_tuples == 0) {
        return NULL;
    }

    for (size_t i = 0; i < num_tuples; i++) {
        if (tuples[i].size > 0 && tuples[i].elements == NULL) {
            return NULL;
        }
        if (tuples[i].size > SIZE_MAX / sizeof(int)) {
            return NULL;
        }
    }

    normalized = calloc(num_tuples, sizeof(Tuple));
    if (normalized == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < num_tuples; i++) {
        if (duplicate_tuple(&tuples[i], &normalized[i]) != 0) {
            for (size_t j = 0; j < i; j++) {
                free(normalized[j].elements);
            }
            free(normalized);
            return NULL;
        }
        normalize_tuple(&normalized[i]);
    }

    result = calloc(num_tuples, sizeof(TupleFreq));
    if (result == NULL) {
        for (size_t i = 0; i < num_tuples; i++) {
            free(normalized[i].elements);
        }
        free(normalized);
        return NULL;
    }

    for (size_t i = 0; i < num_tuples; i++) {
        int found = 0;
        for (size_t j = 0; j < unique_count; j++) {
            if (tuples_equal(&normalized[i], &result[j].tuple)) {
                result[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (duplicate_tuple(&normalized[i], &result[unique_count].tuple) != 0) {
                for (size_t k = 0; k < unique_count; k++) {
                    free(result[k].tuple.elements);
                }
                free(result);
                for (size_t k = 0; k < num_tuples; k++) {
                    free(normalized[k].elements);
                }
                free(normalized);
                return NULL;
            }
            result[unique_count].count = 1;
            unique_count++;
        }
    }

    for (size_t i = 0; i < num_tuples; i++) {
        free(normalized[i].elements);
    }
    free(normalized);

    *result_size = unique_count;
    return result;
}

void free_tuple_frequencies(TupleFreq *freqs, size_t size)
{
    if (freqs == NULL) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        free(freqs[i].tuple.elements);
    }
    free(freqs);
}

int main(void)
{
    int t1[] = {1, 2, 3};
    int t2[] = {3, 2, 1};
    int t3[] = {4, 5};
    int t4[] = {2, 1, 3};
    int t5[] = {5, 4};

    Tuple tuples[] = {
        {t1, 3},
        {t2, 3},
        {t3, 2},
        {t4, 3},
        {t5, 2}
    };

    size_t result_size = 0;
    TupleFreq *freqs = get_tuple_frequencies(tuples, sizeof(tuples) / sizeof(tuples[0]), &result_size);

    if (freqs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Unique tuple frequencies (order irrespective):\n");
    for (size_t i = 0; i < result_size; i++) {
        printf("Tuple [");
        for (size_t j = 0; j < freqs[i].tuple.size; j++) {
            printf("%d", freqs[i].tuple.elements[j]);
            if (j + 1 < freqs[i].tuple.size) {
                printf(", ");
            }
        }
        printf("] : %zu\n", freqs[i].count);
    }

    free_tuple_frequencies(freqs, result_size);
    return EXIT_SUCCESS;
}