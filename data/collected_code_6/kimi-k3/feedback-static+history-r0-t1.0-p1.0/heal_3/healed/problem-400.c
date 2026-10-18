#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    for (size_t i = 0; i < a->size; i++) {
        if (a->elements[i] != b->elements[i]) {
            return 0;
        }
    }
    return 1;
}

static void normalize_tuple(Tuple *t)
{
    qsort(t->elements, t->size, sizeof(int), compare_ints);
}

static int safe_memcpy(void *dest, size_t dest_size, const void *src, size_t src_size)
{
    if (src_size > dest_size) {
        return 0;
    }
    if (src_size > 0 && (dest == NULL || src == NULL)) {
        return 0;
    }
    if (src_size > 0) {
        memcpy(dest, src, src_size);
    }
    return 1;
}

TupleFreq *get_tuple_frequencies(Tuple *tuples, size_t num_tuples, size_t *result_size)
{
    if (tuples == NULL || result_size == NULL || num_tuples == 0) {
        return NULL;
    }

    if (num_tuples > SIZE_MAX / sizeof(Tuple)) {
        return NULL;
    }

    Tuple *normalized = malloc(num_tuples * sizeof(Tuple));
    if (normalized == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < num_tuples; i++) {
        normalized[i].size = tuples[i].size;
        
        if (tuples[i].size > 0 && tuples[i].elements == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(normalized[j].elements);
            }
            free(normalized);
            return NULL;
        }

        if (tuples[i].size > SIZE_MAX / sizeof(int)) {
            for (size_t j = 0; j < i; j++) {
                free(normalized[j].elements);
            }
            free(normalized);
            return NULL;
        }

        size_t copy_size = tuples[i].size * sizeof(int);
        normalized[i].elements = malloc(copy_size);
        
        if (normalized[i].elements == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(normalized[j].elements);
            }
            free(normalized);
            return NULL;
        }

        if (!safe_memcpy(normalized[i].elements, copy_size, tuples[i].elements, copy_size)) {
            free(normalized[i].elements);
            for (size_t j = 0; j < i; j++) {
                free(normalized[j].elements);
            }
            free(normalized);
            return NULL;
        }
        normalize_tuple(&normalized[i]);
    }

    if (num_tuples > SIZE_MAX / sizeof(TupleFreq)) {
        for (size_t i = 0; i < num_tuples; i++) {
            free(normalized[i].elements);
        }
        free(normalized);
        return NULL;
    }

    TupleFreq *result = malloc(num_tuples * sizeof(TupleFreq));
    if (result == NULL) {
        for (size_t i = 0; i < num_tuples; i++) {
            free(normalized[i].elements);
        }
        free(normalized);
        return NULL;
    }

    size_t unique_count = 0;
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
            result[unique_count].tuple.size = normalized[i].size;
            
            if (normalized[i].size > SIZE_MAX / sizeof(int)) {
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

            size_t copy_size = normalized[i].size * sizeof(int);
            result[unique_count].tuple.elements = malloc(copy_size);
            
            if (result[unique_count].tuple.elements == NULL) {
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

            if (!safe_memcpy(result[unique_count].tuple.elements, copy_size, normalized[i].elements, copy_size)) {
                free(result[unique_count].tuple.elements);
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
    TupleFreq *freqs = get_tuple_frequencies(tuples, 5, &result_size);
    
    if (freqs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Unique tuple frequencies (order irrespective):\n");
    for (size_t i = 0; i < result_size; i++) {
        printf("Tuple [");
        for (size_t j = 0; j < freqs[i].tuple.size; j++) {
            printf("%d", freqs[i].tuple.elements[j]);
            if (j < freqs[i].tuple.size - 1) {
                printf(", ");
            }
        }
        printf("] : %zu\n", freqs[i].count);
    }

    free_tuple_frequencies(freqs, result_size);
    return EXIT_SUCCESS;
}