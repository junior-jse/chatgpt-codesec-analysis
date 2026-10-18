#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    size_t length;
} Tuple;

bool all_tuples_equal_length(Tuple *tuples, size_t num_tuples) {
    if (tuples == NULL || num_tuples == 0) {
        return false;
    }

    size_t expected_length = tuples[0].length;

    for (size_t i = 0; i < num_tuples; i++) {
        if (tuples[i].elements == NULL) {
            return false;
        }
        if (tuples[i].length != expected_length) {
            return false;
        }
    }

    return true;
}

int main(void) {
    int a[] = {1, 2, 3};
    int b[] = {4, 5, 6};
    int c[] = {7, 8, 9};
    
    Tuple t1 = {a, 3};
    Tuple t2 = {b, 3};
    Tuple t3 = {c, 3};
    
    Tuple tuples1[] = {t1, t2, t3};
    
    if (all_tuples_equal_length(tuples1, 3)) {
        printf("All tuples have equal length.\n");
    } else {
        printf("Tuples do not have equal length.\n");
    }

    int d[] = {10, 11};
    Tuple t4 = {d, 2};
    
    Tuple tuples2[] = {t1, t4};
    
    if (all_tuples_equal_length(tuples2, 2)) {
        printf("All tuples have equal length.\n");
    } else {
        printf("Tuples do not have equal length.\n");
    }

    return EXIT_SUCCESS;
}