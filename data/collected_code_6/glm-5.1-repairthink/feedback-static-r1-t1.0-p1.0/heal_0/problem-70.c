#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    size_t length;
} Tuple;

bool check_equal_length(Tuple *tuples, size_t count) {
    if (count == 0) {
        return true;
    }

    size_t expected_length = tuples[0].length;

    for (size_t i = 0; i < count; i++) {
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
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    int a3[] = {7, 8, 9};

    Tuple t1 = {a1, 3};
    Tuple t2 = {a2, 3};
    Tuple t3 = {a3, 3};

    Tuple equal_tuples[] = {t1, t2, t3};
    size_t equal_count = sizeof(equal_tuples) / sizeof(equal_tuples[0]);

    bool result1 = check_equal_length(equal_tuples, equal_count);
    printf("%d\n", result1);

    int a4[] = {10, 11};
    Tuple t4 = {a4, 2};

    Tuple unequal_tuples[] = {t1, t4};
    size_t unequal_count = sizeof(unequal_tuples) / sizeof(unequal_tuples[0]);

    bool result2 = check_equal_length(unequal_tuples, unequal_count);
    printf("%d\n", result2);

    return 0;
}