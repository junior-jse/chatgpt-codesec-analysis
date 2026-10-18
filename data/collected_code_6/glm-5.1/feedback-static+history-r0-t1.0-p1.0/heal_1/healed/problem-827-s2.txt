#include <stdio.h>
#include <stdlib.h>

int sum_column(int * const *lists, const int *sizes, int num_lists, int col_index) {
    int total = 0;
    if (lists == NULL || sizes == NULL) {
        return 0;
    }
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && col_index >= 0 && col_index < sizes[i]) {
            total += lists[i][col_index];
        }
    }
    return total;
}

int main(void) {
    int row0[] = {1, 2, 3};
    int row1[] = {4, 5};
    int row2[] = {7, 8, 9, 10};

    int num_lists = 3;
    int *lists[] = {row0, row1, row2};
    int sizes[] = {3, 2, 4};

    int col_to_sum = 1;
    int result = sum_column(lists, sizes, num_lists, col_to_sum);

    printf("%d\n", result);

    return 0;
}