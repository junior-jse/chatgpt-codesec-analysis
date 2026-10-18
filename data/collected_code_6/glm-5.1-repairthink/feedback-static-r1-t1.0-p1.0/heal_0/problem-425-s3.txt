#include <stdio.h>
#include <stdlib.h>

int count_sublists(int** lists, int* sizes, int list_count, int target) {
    int count = 0;
    for (int i = 0; i < list_count; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }
        int found = 0;
        for (int j = 0; j < sizes[i]; j++) {
            if (lists[i][j] == target) {
                found = 1;
                break;
            }
        }
        count += found;
    }
    return count;
}

int main() {
    int list_count = 3;
    int** lists = (int**)malloc(list_count * sizeof(int*));
    int* sizes = (int*)malloc(list_count * sizeof(int));
    
    if (lists == NULL || sizes == NULL) {
        free(lists);
        free(sizes);
        return 1;
    }

    sizes[0] = 3;
    lists[0] = (int*)malloc(sizes[0] * sizeof(int));
    if (lists[0] == NULL) { 
        free(lists); 
        free(sizes); 
        return 1; 
    }
    lists[0][0] = 1;
    lists[0][1] = 2;
    lists[0][2] = 3;

    sizes[1] = 4;
    lists[1] = (int*)malloc(sizes[1] * sizeof(int));
    if (lists[1] == NULL) { 
        for(int i = 0; i < 1; i++) free(lists[i]);
        free(lists); 
        free(sizes); 
        return 1; 
    }
    lists[1][0] = 4;
    lists[1][1] = 5;
    lists[1][2] = 5;
    lists[1][3] = 6;

    sizes[2] = 2;
    lists[2] = (int*)malloc(sizes[2] * sizeof(int));
    if (lists[2] == NULL) { 
        for(int i = 0; i < 2; i++) free(lists[i]);
        free(lists); 
        free(sizes); 
        return 1; 
    }
    lists[2][0] = 7;
    lists[2][1] = 8;

    int target1 = 5;
    int result1 = count_sublists(lists, sizes, list_count, target1);
    printf("%d\n", result1);

    int target2 = 10;
    int result2 = count_sublists(lists, sizes, list_count, target2);
    printf("%d\n", result2);

    for (int i = 0; i < list_count; i++) {
        free(lists[i]);
    }
    free(lists);
    free(sizes);

    return 0;
}