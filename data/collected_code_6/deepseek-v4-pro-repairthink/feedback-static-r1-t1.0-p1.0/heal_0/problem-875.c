#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int first;
    int second;
} Tuple;

int min_tuple_difference(Tuple *tuples, int count, int *min_diff) {
    if (tuples == NULL || count <= 0 || min_diff == NULL) {
        return -1;
    }

    *min_diff = INT_MAX;
    int found = 0;

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (i == j) {
                continue;
            }
            int diff = abs(tuples[i].first - tuples[j].first) + 
                       abs(tuples[i].second - tuples[j].second);
            if (diff < *min_diff) {
                *min_diff = diff;
                found = 1;
            }
        }
    }

    if (!found) {
        return -1;
    }

    return 0;
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {15, 6},
        {7, 8}
    };
    int count = sizeof(tuples) / sizeof(tuples[0]);
    int min_diff;

    if (min_tuple_difference(tuples, count, &min_diff) == 0) {
        printf("Minimum difference: %d\n", min_diff);
    } else {
        printf("Error calculating minimum difference\n");
    }

    Tuple single_tuple[] = {{5, 5}};
    if (min_tuple_difference(single_tuple, 1, &min_diff) == 0) {
        printf("Minimum difference: %d\n", min_diff);
    } else {
        printf("Error calculating minimum difference\n");
    }

    if (min_tuple_difference(NULL, count, &min_diff) == 0) {
        printf("Minimum difference: %d\n", min_diff);
    } else {
        printf("Error calculating minimum difference\n");
    }

    return 0;
}