#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int* find_max_sum_list(int** lists, const int* sizes, int num_lists) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return NULL;
    }
    
    int max_sum = INT_MIN;
    int max_index = -1;
    
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
            max_index = i;
        }
    }
    
    if (max_index == -1) {
        return NULL;
    }
    
    return lists[max_index];
}

int main() {
    int list1[] = {1, 2, 3, 4, 5};
    int list2[] = {-5, -4, -3, -2, -1};
    int list3[] = {10, 20};
    int list4[] = {7, 8, 9, 10};
    
    int* lists[] = {list1, list2, list3, list4};
    /* Possible weaknesses found:
     *  Variable 'sizes' can be declared as const array [constVariable]
     */
    int sizes[] = {5, 5, 2, 4};
    
    const int* max_list = find_max_sum_list(lists, sizes, 4);
    
    if (max_list != NULL) {
        printf("Maximum sum list: ");
        int max_size = 0;
        if (max_list == list1 || max_list == list2) {
            max_size = sizes[0];
        } else if (max_list == list3) {
            max_size = sizes[2];
        } else if (max_list == list4) {
            max_size = sizes[3];
        }
        for (int i = 0; i < max_size; i++) {
            printf("%d ", max_list[i]);
        }
        printf("\n");
    } else {
        printf("No valid list found\n");
    }
    
    return 0;
}