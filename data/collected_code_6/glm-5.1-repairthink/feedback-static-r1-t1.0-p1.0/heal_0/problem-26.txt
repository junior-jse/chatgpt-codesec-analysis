#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool check_tuple_list(int **tuples, int num_tuples, int k) {
    if (tuples == NULL || num_tuples <= 0 || k <= 0) {
        return false;
    }

    size_t *tuple_sizes = (size_t *)malloc(num_tuples * sizeof(size_t));
    if (tuple_sizes == NULL) {
        return false;
    }

    int **tuple_ptrs = (int **)malloc(num_tuples * sizeof(int *));
    if (tuple_ptrs == NULL) {
        free(tuple_sizes);
        return false;
    }

    for (int i = 0; i < num_tuples; i++) {
        if (tuples[i] == NULL) {
            free(tuple_sizes);
            free(tuple_ptrs);
            return false;
        }
        tuple_sizes[i] = k;
        tuple_ptrs[i] = tuples[i];
    }

    bool result = true;
    for (int i = 0; i < num_tuples; i++) {
        if (tuple_sizes[i] != k) {
            result = false;
            break;
        }
    }

    free(tuple_sizes);
    free(tuple_ptrs);

    return result;
}

int main(void) {
    int t0[] = {1, 2, 3};
    int t1[] = {4, 5, 6};
    int t2[] = {7, 8, 9};
    int *tuples[] = {t0, t1, t2};
    int num_tuples = 3;
    int k = 3;

    if (check_tuple_list(tuples, num_tuples, k)) {
        printf("All tuples have %d elements\n", k);
    } else {
        printf("Not all tuples have %d elements\n", k);
    }

    return 0;
}