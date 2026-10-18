#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int count;
} Record;

typedef struct {
    Record *items;
    int size;
} Tuple;

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int check_similar_occurrences(const Tuple *tuples, int num_tuples) {
    if (tuples == NULL || num_tuples <= 0) {
        return -1;
    }

    int *counts = (int *)malloc(num_tuples * sizeof(int));
    if (counts == NULL) {
        return -1;
    }

    for (int i = 0; i < num_tuples; ++i) {
        if (tuples[i].items == NULL || tuples[i].size <= 0) {
            free(counts);
            return -1;
        }
        counts[i] = tuples[i].size;
    }

    qsort(counts, num_tuples, sizeof(int), compare_ints);

    int unique_groups = 0;
    int group_counts[num_tuples];
    int current_count = counts[0];
    int group_len = 1;

    for (int i = 1; i < num_tuples; ++i) {
        if (counts[i] == current_count) {
            group_len++;
        } else {
            group_counts[unique_groups++] = group_len;
            current_count = counts[i];
            group_len = 1;
        }
    }
    group_counts[unique_groups++] = group_len;

    qsort(group_counts, unique_groups, sizeof(int), compare_ints);

    free(counts);

    return unique_groups == 1 ? 1 : (unique_groups == 2 && group_counts[0] == group_counts[1] ? 1 : 0);
}

int main(void) {
    Record r1[] = {{1, 10}, {2, 20}, {3, 30}};
    Record r2[] = {{4, 40}, {5, 50}};
    Record r3[] = {{6, 60}, {7, 70}};
    Record r4[] = {{8, 80}};

    Tuple tuples[] = {
        {r1, 3},
        {r2, 2},
        {r3, 2},
        {r4, 1}
    };

    int result = check_similar_occurrences(tuples, 4);
    printf("Result: %d\n", result);

    Tuple tuples2[] = {
        {r1, 3},
        {r2, 2},
        {r3, 2},
        {r4, 1},
        {r1, 3}
    };

    result = check_similar_occurrences(tuples2, 5);
    printf("Result: %d\n", result);

    return 0;
}