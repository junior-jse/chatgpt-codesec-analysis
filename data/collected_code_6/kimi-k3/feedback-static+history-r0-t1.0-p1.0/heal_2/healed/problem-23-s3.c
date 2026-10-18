#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_max_sum(int **lists, int num_lists, const int *sizes, int *max_sum) {
    if (lists == NULL || sizes == NULL || max_sum == NULL || num_lists <= 0) {
        return -1;
    }
    
    *max_sum = INT_MIN;
    
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }
        
        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            if (current_sum > INT_MAX - lists[i][j]) {
                return -2;
            }
            current_sum += lists[i][j];
        }
        
        if (current_sum > *max_sum) {
            *max_sum = current_sum;
        }
    }
    
    if (*max_sum == INT_MIN) {
        return -3;
    }
    
    return 0;
}

int main(void) {
    int list1[] = {1, 2, 3, 4};
    int list2[] = {10, 20, 30};
    int list3[] = {5, 5, 5};
    int list4[] = {-1, -2, -3};
    
    int *lists[] = {list1, list2, list3, list4};
    const int sizes[] = {4, 3, 3, 3};
    int num_lists = 4;
    int max_sum;
    
    int result = find_max_sum(lists, num_lists, sizes, &max_sum);
    
    if (result == 0) {
        printf("Maximum sum: %d\n", max_sum);
    } else {
        printf("Error: %d\n", result);
    }
    
    return 0;
}