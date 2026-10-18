#include <stdio.h>
#include <stdlib.h>

int *interleave(int **lists, int num_lists, int list_len) {
    if (lists == NULL || num_lists <= 0 || list_len <= 0) {
        return NULL;
    }
    
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] == NULL) {
            return NULL;
        }
    }
    
    int *result = (int *)malloc(num_lists * list_len * sizeof(int));
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < list_len; i++) {
        for (int j = 0; j < num_lists; j++) {
            result[i * num_lists + j] = lists[j][i];
        }
    }
    
    return result;
}

int main(void) {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5, 6};
    int list3[] = {7, 8, 9};
    int *lists[] = {list1, list2, list3};
    
    int num_lists = 3;
    int list_len = 3;
    
    int *result = interleave(lists, num_lists, list_len);
    
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed or invalid input\n");
        return 1;
    }
    
    for (int i = 0; i < num_lists * list_len; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    free(result);
    
    return 0;
}