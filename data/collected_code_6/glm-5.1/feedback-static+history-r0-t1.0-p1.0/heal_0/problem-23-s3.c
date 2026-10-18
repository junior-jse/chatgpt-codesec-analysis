#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int max_sum_list(int **lists, int *sizes, int num_lists) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return INT_MIN;
    }

    int max_sum = INT_MIN;

    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            continue;
        }

        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (current_sum > max_sum) {
            max_sum = current_sum;
        }
    }

    return max_sum;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {-1, -2, -3};
    
    int num_lists = 3;
    int *lists[] = {list1, list2, list3};
    int sizes[] = {3, 2, 3};

    int result = max_sum_list(lists, sizes, num_lists);
    
    printf("%d\n", result);

    return 0;
}