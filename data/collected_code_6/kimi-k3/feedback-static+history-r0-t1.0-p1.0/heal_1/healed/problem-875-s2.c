#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

int find_min_difference(const Tuple *tuples, size_t count, int *min_diff) {
    size_t i;
    int current_min;
    
    if (tuples == NULL || min_diff == NULL || count == 0) {
        return -1;
    }
    
    current_min = abs(tuples[0].first - tuples[0].second);
    
    for (i = 1; i < count; i++) {
        int diff = abs(tuples[i].first - tuples[i].second);
        if (diff < current_min) {
            current_min = diff;
        }
    }
    
    *min_diff = current_min;
    return 0;
}

int main(void) {
    Tuple tuples[] = {{10, 25}, {5, 8}, {100, 95}, {50, 70}};
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    int min_diff;
    int result;
    
    result = find_min_difference(tuples, count, &min_diff);
    
    if (result == 0) {
        printf("Minimum difference: %d\n", min_diff);
    } else {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}