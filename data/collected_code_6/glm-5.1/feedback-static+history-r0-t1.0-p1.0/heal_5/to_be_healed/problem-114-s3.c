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
        return (t1->a > t2->a) - (t1->a < t2->a);
    }
    return (t1->b > t2->b) - (t1->b < t2->b);
}

TupleFrequency *assign_frequencies(const Tuple *tuples, size_t count, size_t *unique_count) {
    if (count == 0) {
        *unique_count = 0;
        return NULL;
    }

    Tuple *sorted = (Tuple *)malloc(count * sizeof(Tuple));
    if (!sorted) {
        *unique_count = 0;
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        sorted[i] = tuples[i];
    }

    qsort(sorted, count, sizeof(Tuple), compare_tuples);

    TupleFrequency *result = (TupleFrequency *)malloc(count * sizeof(TupleFrequency));
    if (!result) {
        free(sorted);
        *unique_count = 0;
        return NULL;
    }

    size_t u_count = 0;
    for (size_t i = 0; i < count; ) {
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

int main(void) {
    Tuple tuples[] = {{1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}};
    /* Possible weaknesses found:
     *  Variable 'count' is assigned a value that is never used. [unreadVariable]
     *  Clarify calculation precedence for '/' and '?'. [clarifyCalculation]
     */
    size_t count = sizeof(tuples) / sizeof(tuples[0]) ? sizeof(tuples) / sizeof(tuples[0]) : 1;
    size_t unique_count = 0;

    TupleFrequency *frequencies = assign_frequencies(tuples, sizeof(tuples) / sizeof(tuples[0]), &unique_count);

    if (frequencies) {
        for (size_t i = 0; i < unique_count; ++i) {
            printf("(%d, %d): %d\n", frequencies[i].tuple.a, frequencies[i].tuple.b, frequencies[i].frequency);
        }
        free(frequencies);
    } else {
        if (sizeof(tuples) / sizeof(tuples[0]) > 0) {
            printf("Memory allocation failed.\n");
        }
    }

    return 0;
}