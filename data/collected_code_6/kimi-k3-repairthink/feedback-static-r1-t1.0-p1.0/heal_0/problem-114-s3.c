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

int compare_tuples(const void *a, const void *b) {
    const Tuple *t1 = (const Tuple *)a;
    const Tuple *t2 = (const Tuple *)b;
    
    if (t1->a != t2->a) {
        return (t1->a < t2->a) ? -1 : 1;
    }
    if (t1->b != t2->b) {
        return (t1->b < t2->b) ? -1 : 1;
    }
    return 0;
}

TupleFrequency *assign_frequency(const Tuple *tuples, size_t count, size_t *result_count) {
    if (tuples == NULL || result_count == NULL || count == 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    Tuple *sorted = malloc(count * sizeof(Tuple));
    if (sorted == NULL) {
        *result_count = 0;
        return NULL;
    }
    memcpy(sorted, tuples, count * sizeof(Tuple));
    qsort(sorted, count, sizeof(Tuple), compare_tuples);

    TupleFrequency *result = malloc(count * sizeof(TupleFrequency));
    if (result == NULL) {
        free(sorted);
        *result_count = 0;
        return NULL;
    }

    size_t unique_count = 0;
    result[unique_count].tuple = sorted[0];
    result[unique_count].frequency = 1;

    for (size_t i = 1; i < count; i++) {
        if (compare_tuples(&sorted[i], &sorted[i - 1]) == 0) {
            result[unique_count].frequency++;
        } else {
            unique_count++;
            result[unique_count].tuple = sorted[i];
            result[unique_count].frequency = 1;
        }
    }

    free(sorted);
    *result_count = unique_count + 1;
    return result;
}

int main(void) {
    Tuple tuples[] = {
        {1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}, {7, 8}
    };
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    size_t result_count = 0;

    TupleFrequency *frequencies = assign_frequency(tuples, count, &result_count);
    
    if (frequencies == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory or invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Tuple frequencies:\n");
    for (size_t i = 0; i < result_count; i++) {
        printf("(%d, %d): %d\n", 
               frequencies[i].tuple.a, 
               frequencies[i].tuple.b, 
               frequencies[i].frequency);
    }

    free(frequencies);
    return EXIT_SUCCESS;
}