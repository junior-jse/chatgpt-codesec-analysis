#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int* find_max_sum_list(int** lists, int num_lists, const int* sizes, int* result_size) {
    int max_sum = INT_MIN;
    int* max_list = NULL;
    *result_size = 0;

    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return NULL;
    }

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }

        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (current_sum > max_sum) {
            max_sum = current_sum;
            max_list = lists[i];
            *result_size = sizes[i];
        }
    }

    return max_list;
}

int main(void) {
    int list1[] = {1, 2, 3, 4};
    int list2[] = {10, 20, 30};
    int list3[] = {5, 5, 5, 5, 5};
    int list4[] = {100};

    int* lists[] = {list1, list2, list3, list4};
    /* Possible weaknesses found:
     *  Variable 'sizes' can be declared as const array [constVariable]
     */
    int sizes[] = {4, 3, 5, 1};
    int num_lists = 4;
    int result_size;

    const int* result = find_max_sum_list(lists, num_lists, sizes, &result_size);

    if (result != NULL) {
        printf("List with highest sum: ");
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    } else {
        printf("No valid list found.\n");
    }

    return 0;
}