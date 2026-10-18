#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int first;
    int second;
} Tuple;

int find_min_difference(const Tuple *tuples, size_t count, int *min_diff) {
    size_t i;
    int diff;
    int current_min;
    
    if (tuples == NULL || count == 0 || min_diff == NULL) {
        return -1;
    }
    
    current_min = INT_MAX;
    
    for (i = 0; i < count; i++) {
        diff = abs(tuples[i].first - tuples[i].second);
        if (diff < current_min) {
            current_min = diff;
        }
    }
    
    *min_diff = current_min;
    return 0;
}

int main(void) {
    Tuple tuples[] = {{10, 3}, {5, 8}, {20, 12}, {7, 7}};
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    int min_diff;
    
    if (find_min_difference(tuples, count, &min_diff) == 0) {
        printf("Minimum difference: %d\n", min_diff);
    } else {
        fprintf(stderr, "Error calculating minimum difference\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}