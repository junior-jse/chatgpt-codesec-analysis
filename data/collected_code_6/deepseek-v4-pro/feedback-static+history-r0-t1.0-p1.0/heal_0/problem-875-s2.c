#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int first;
    int second;
} Tuple;

int compare_tuples(const void *a, const void *b) {
    const Tuple *t1 = (const Tuple *)a;
    const Tuple *t2 = (const Tuple *)b;
    if (t1->first != t2->first) {
        return t1->first - t2->first;
    }
    return t1->second - t2->second;
}

int min_difference(Tuple *tuples, int size) {
    if (tuples == NULL || size < 2) {
        fprintf(stderr, "Invalid input\n");
        return -1;
    }

    qsort(tuples, size, sizeof(Tuple), compare_tuples);

    int min_diff = INT_MAX;

    for (int i = 0; i < size - 1; i++) {
        int diff = abs(tuples[i + 1].first - tuples[i].first) +
                   abs(tuples[i + 1].second - tuples[i].second);
        if (diff < min_diff) {
            min_diff = diff;
        }
    }

    return min_diff;
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {4, 5},
        {3, 7},
        {6, 8},
        {2, 3}
    };
    int size = sizeof(tuples) / sizeof(tuples[0]);

    int result = min_difference(tuples, size);
    if (result != -1) {
        printf("Minimum difference: %d\n", result);
    }

    return 0;
}