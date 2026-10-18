#include <stdio.h>
#include <stdlib.h>

int all_equal_length(int **tuples, int *lengths, int num_tuples) {
    if (tuples == NULL || lengths == NULL || num_tuples <= 0) {
        return 0;
    }
    
    int first_length = lengths[0];
    for (int i = 1; i < num_tuples; i++) {
        if (lengths[i] != first_length) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int tuple1[] = {1, 2, 3};
    int tuple2[] = {4, 5, 6};
    int tuple3[] = {7, 8, 9};
    
    int *tuples[] = {tuple1, tuple2, tuple3};
    int lengths[] = {3, 3, 3};
    
    if (all_equal_length(tuples, lengths, 3)) {
        printf("All tuples have equal length\n");
    } else {
        printf("Tuples have different lengths\n");
    }
    
    int tuple4[] = {10, 11};
    int lengths2[] = {3, 3, 2};
    int *tuples2[] = {tuple1, tuple2, tuple4};
    
    if (all_equal_length(tuples2, lengths2, 3)) {
        printf("All tuples have equal length\n");
    } else {
        printf("Tuples have different lengths\n");
    }
    
    return 0;
}