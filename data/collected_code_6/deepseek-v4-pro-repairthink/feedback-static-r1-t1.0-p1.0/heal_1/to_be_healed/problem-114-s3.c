#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    int size;
} Tuple;

typedef struct {
    Tuple tuple;
    int frequency;
} TupleFrequency;

int tuple_equals(const Tuple *a, const Tuple *b) {
    if (a->size != b->size) return 0;
    for (int i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

TupleFrequency* assign_frequencies(const Tuple *tuples, int tuple_count, int *result_count) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_count==NULL' is not redundant
     */
    if (tuples == NULL || tuple_count <= 0 || result_count == NULL) {
        /* Possible weaknesses found:
         *  Either the condition 'result_count==NULL' is redundant or there is possible null pointer dereference: result_count. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_count = 0;
        return NULL;
    }

    TupleFrequency *result = malloc(tuple_count * sizeof(TupleFrequency));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int unique_count = 0;

    for (int i = 0; i < tuple_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (tuple_equals(&result[j].tuple, &tuples[i])) {
                result[j].frequency++;
                found = 1;
                break;
            }
        }
        if (!found) {
            result[unique_count].tuple = tuples[i];
            result[unique_count].frequency = 1;
            unique_count++;
        }
    }

    *result_count = unique_count;
    return result;
}

int main(void) {
    int t1_data[] = {1, 2, 3};
    int t2_data[] = {4, 5, 6};
    int t3_data[] = {1, 2, 3};
    int t4_data[] = {7, 8};

    /* Possible weaknesses found:
     *  Variable 'tuples' can be declared as const array [constVariable]
     */
    Tuple tuples[] = {
        {t1_data, 3},
        {t2_data, 3},
        {t3_data, 3},
        {t4_data, 2}
    };

    int result_count = 0;
    TupleFrequency *frequencies = assign_frequencies(tuples, 4, &result_count);

    if (frequencies != NULL) {
        for (int i = 0; i < result_count; i++) {
            printf("Tuple (");
            for (int j = 0; j < frequencies[i].tuple.size; j++) {
                printf("%d", frequencies[i].tuple.data[j]);
                if (j < frequencies[i].tuple.size - 1) printf(", ");
            }
            printf(") -> frequency: %d\n", frequencies[i].frequency);
        }
        free(frequencies);
    } else {
        printf("No tuples to process.\n");
    }

    return 0;
}