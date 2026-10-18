#include <stdio.h>
#include <stdlib.h>

int sum_column(int **lists, int *sizes, int num_lists, int col_index) {
    int sum = 0;
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return 0;
    }
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && col_index >= 0 && col_index < sizes[i]) {
            sum += lists[i][col_index];
        }
    }
    return sum;
}

int main(void) {
    int sizes[] = {3, 4, 2};
    int num_lists = 3;
    int col_index = 1;
    
    int **lists = (int **)malloc(num_lists * sizeof(int *));
    if (lists == NULL) {
        return EXIT_FAILURE;
    }

    lists[0] = (int *)malloc(sizes[0] * sizeof(int));
    lists[1] = (int *)malloc(sizes[1] * sizeof(int));
    lists[2] = (int *)malloc(sizes[2] * sizeof(int));

    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        for (int i = 0; i < num_lists; i++) {
            free(lists[i]);
        }
        free(lists);
        return EXIT_FAILURE;
    }

    lists[0][0] = 1; lists[0][1] = 2; lists[0][2] = 3;
    lists[1][0] = 4; lists[1][1] = 5; lists[1][2] = 6; lists[1][3] = 7;
    lists[2][0] = 8; lists[2][1] = 9;

    int result = sum_column(lists, sizes, num_lists, col_index);
    printf("%d\n", result);

    for (int i = 0; i < num_lists; i++) {
        free(lists[i]);
    }
    free(lists);

    return EXIT_SUCCESS;
}