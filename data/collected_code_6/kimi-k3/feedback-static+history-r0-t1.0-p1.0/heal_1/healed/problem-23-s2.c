#include <stdio.h>
#include <stdlib.h>

int max_sum_of_sublists(int **lists, int num_lists, const int *sizes) {
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return -1;
    }
    
    int max_sum = 0;
    int found_valid_list = 0;
    
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }
        
        int current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }
        
        if (!found_valid_list || current_sum > max_sum) {
            max_sum = current_sum;
            found_valid_list = 1;
        }
    }
    
    if (!found_valid_list) {
        return -1;
    }
    
    return max_sum;
}

int main(void) {
    int *list1 = malloc(3 * sizeof(int));
    int *list2 = malloc(4 * sizeof(int));
    int *list3 = malloc(2 * sizeof(int));
    
    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        free(list1);
        free(list2);
        free(list3);
        return 1;
    }
    
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    
    list2[0] = 10;
    list2[1] = 20;
    list2[2] = 30;
    list2[3] = 40;
    
    list3[0] = -5;
    list3[1] = -10;
    
    int **lists = malloc(3 * sizeof(int *));
    int *sizes = malloc(3 * sizeof(int));
    
    if (lists == NULL || sizes == NULL) {
        free(list1);
        free(list2);
        free(list3);
        free(lists);
        free(sizes);
        return 1;
    }
    
    lists[0] = list1;
    lists[1] = list2;
    lists[2] = list3;
    
    sizes[0] = 3;
    sizes[1] = 4;
    sizes[2] = 2;
    
    int result = max_sum_of_sublists(lists, 3, sizes);
    
    if (result != -1) {
        printf("Maximum sum: %d\n", result);
    } else {
        printf("Error: Invalid input or no valid lists\n");
    }
    
    free(list1);
    free(list2);
    free(list3);
    free(lists);
    free(sizes);
    
    return 0;
}