#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple tuple;
    int frequency;
} TupleFrequency;

int compare_tuples(const void *p1, const void *p2) {
    const Tuple *t1 = (const Tuple *)p1;
    const Tuple *t2 = (const Tuple *)p2;
    if (t1->a != t2->a) {
        return t1->a - t2->a;
    }
    return t1->b - t2->b;
}

/* Possible weaknesses found:
 *  Parameter 'tuples' can be declared as pointer to const [constParameterPointer]
 */
TupleFrequency *assign_frequencies(Tuple *tuples, int count, int *unique_count) {
    if (count <= 0) {
        *unique_count = 0;
        return NULL;
    }

    Tuple *sorted = (Tuple *)malloc(count * sizeof(Tuple));
    if (!sorted) {
        *unique_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; ++i) {
        sorted[i] = tuples[i];
    }

    qsort(sorted, count, sizeof(Tuple), compare_tuples);

    int capacity = count;
    TupleFrequency *result = (TupleFrequency *)malloc(capacity * sizeof(TupleFrequency));
    if (!result) {
        free(sorted);
        *unique_count = 0;
        return NULL;
    }

    int u_count = 0;
    for (int i = 0; i < count; ) {
        Tuple current = sorted[i];
        int freq = 0;
        while (i < count && compare_tuples(&sorted[i], &current) == 0) {
            freq++;
            i++;
        }
        result[u_count].tuple = current;
        result[u_count].frequency = freq;
        u_count++;
    }

    free(sorted);
    *unique_count = u_count;

    TupleFrequency *trimmed = (TupleFrequency *)realloc(result, u_count * sizeof(TupleFrequency));
    if (trimmed) {
        return trimmed;
    }
    return result;
}

int main() {
    Tuple tuples[] = {{1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}};
    /* Possible weaknesses found:
     *  Assignment 'count=sizeof(tuples)/sizeof(tuples[0])', assigned value is 6
     */
    int count = sizeof(tuples) / sizeof(tuples[0]);
    int unique_count = 0;

    TupleFrequency *frequencies = assign_frequencies(tuples, count, &unique_count);

    if (frequencies) {
        for (int i = 0; i < unique_count; ++i) {
            printf("(%d, %d): %d\n", frequencies[i].tuple.a, frequencies[i].tuple.b, frequencies[i].frequency);
        }
        free(frequencies);
    } else {
        /* Possible weaknesses found:
         *  Condition 'count>0' is always true
         *  Condition 'count>0' is always true [knownConditionTrueFalse]
         */
        if (count > 0) {
            printf("Memory allocation failed.\n");
        }
    }

    return 0;
}