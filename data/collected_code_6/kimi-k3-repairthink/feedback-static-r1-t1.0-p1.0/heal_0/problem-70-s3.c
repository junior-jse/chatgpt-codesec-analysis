#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *elements;
    size_t length;
} Tuple;

int all_tuples_equal_length(const Tuple *tuples, size_t count) {
    size_t i;
    size_t expected_length;

    if (tuples == NULL || count == 0) {
        return 0;
    }

    expected_length = tuples[0].length;

    for (i = 1; i < count; i++) {
        if (tuples[i].length != expected_length) {
            return 0;
        }
    }

    return 1;
}

int main(void) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5, 6};
    int data3[] = {7, 8, 9};

    Tuple tuples[3];

    tuples[0].elements = data1;
    tuples[0].length = 3;

    tuples[1].elements = data2;
    tuples[1].length = 3;

    tuples[2].elements = data3;
    tuples[2].length = 3;

    if (all_tuples_equal_length(tuples, 3)) {
        printf("All tuples have equal length.\n");
    } else {
        printf("Tuples do not have equal length.\n");
    }

    return 0;
}