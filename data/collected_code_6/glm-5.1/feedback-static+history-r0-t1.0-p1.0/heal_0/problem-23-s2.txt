#include <stdio.h>
#include <stdlib.h>

int find_max_sum(int **lists, int num_lists, const int *sizes) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return 0;
    }

    int max_sum = 0;
    int initialized = 0;

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] < 0) {
            continue;
        }

        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (!initialized || current_sum > max_sum) {
            max_sum = current_sum;
            initialized = 1;
        }
    }

    return max_sum;
}

int main(void) {
    int sizes[] = {5, 4, 3};
    int num_lists = sizeof(sizes) / sizeof(sizes[0]);

    int **lists = (int **)malloc(num_lists * sizeof(int *));
    if (lists == NULL) {
        return 1;
    }

    lists[0] = (int *)malloc(sizes[0] * sizeof(int));
    lists[1] = (int *)malloc(sizes[1] * sizeof(int));
    lists[2] = (int *)malloc(sizes[2] * sizeof(int));

    if (lists[0] == NULL || lists[1] == NULL || lists[2] == NULL) {
        free(lists[0]);
        free(lists[1]);
        free(lists[2]);
        free(lists);
        return 1;
    }

    int a0[] = {1, 2, 3, 4, 5};
    int a1[] = {-1, -2, -3, -4};
    int a2[] = {10, 20, 30};

    for (int i = 0; i < sizes[0]; i++) lists[0][i] = a0[i];
    for (int i = 0; i < sizes[1]; i++) lists[1][i] = a1[i];
    for (int i = 0; i < sizes[2]; i++) lists[2][i] = a2[i];

    int max_sum = find_max_sum(lists, num_lists, sizes);
    printf("%d\n", max_sum);

    free(lists[0]);
    free(lists[1]);
    free(lists[2]);
    free(lists);

    return 0;
}