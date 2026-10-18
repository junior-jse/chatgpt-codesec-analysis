#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Possible weaknesses found:
 *  Parameter 'sizes' can be declared as pointer to const [constParameterPointer]
 */
int find_max_sum_list(int **lists, int *sizes, int num_lists) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return -1;
    }
    
    int max_sum = INT_MIN;
    int max_index = -1;
    
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] < 0) {
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
    
    return max_index;
}

int main(void) {
    int list1[] = {1, 2, 3, 4, 5};
    int list2[] = {-1, -2, -3};
    int list3[] = {10, 20};
    int list4[] = {0, 0, 0};
    
    int *lists[] = {list1, list2, list3, list4};
    int sizes[] = {5, 3, 2, 3};
    int num_lists = 4;
    
    int result = find_max_sum_list(lists, sizes, num_lists);
    
    if (result >= 0) {
        printf("List with highest sum: %d\n", result);
    } else {
        printf("No valid list found\n");
    }
    
    return 0;
}