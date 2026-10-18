#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_max_sum(int **lists, int num_lists, const int *list_sizes) {
    if (lists == NULL || list_sizes == NULL || num_lists <= 0) {
        return 0;
    }

    int max_sum = INT_MIN;

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || list_sizes[i] <= 0) {
            continue;
        }

        int current_sum = 0;
        for (int j = 0; j < list_sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (current_sum > max_sum) {
            max_sum = current_sum;
        }
    }

    if (max_sum == INT_MIN) {
        return 0;
    }

    return max_sum;
}

int main(void) {
    const int sizes[] = {4, 5, 3};
    int num_lists = 3;

    int **lists = (int **)malloc(num_lists * sizeof(int *));
    if (lists == NULL) {
        return EXIT_FAILURE;
    }

    lists[0] = (int *)malloc(sizes[0] * sizeof(int));
    lists[1] = (int *)malloc(sizes[1] * sizeof(int));
    lists[2] = (int *)malloc(sizes[2] * sizeof(int));

    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        free(lists[0]);
        free(lists[1]);
        free(lists[2]);
        free(lists);
        return EXIT_FAILURE;
    }

    lists[0][0] = 1; lists[0][1] = 2; lists[0][2] = 3; lists[0][3] = 4;
    lists[1][0] = 10; lists[1][1] = -2; lists[1][2] = 3; lists[1][3] = 1; lists[1][4] = 5;
    lists[2][0] = -1; lists[2][1] = -2; lists[2][2] = -3;

    int result = find_max_sum(lists, num_lists, sizes);
    printf("%d\n", result);

    free(lists[0]);
    free(lists[1]);
    free(lists[2]);
    free(lists);

    return EXIT_SUCCESS;
}