#include <stdio.h>
#include <stdlib.h>

int max_length(int **lists, const int *sizes, int num_lists) {
    int max_len = 0;
    if (lists == NULL || sizes == NULL || num_lists <= 0) {
        return 0;
    }
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && sizes[i] > max_len) {
            max_len = sizes[i];
        }
    }
    return max_len;
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {6};
    
    int *lists[] = {list1, list2, list3};
    const int sizes[] = {sizeof(list1) / sizeof(list1[0]), sizeof(list2) / sizeof(list2[0]), sizeof(list3) / sizeof(list3[0])};
    int num_lists = sizeof(lists) / sizeof(lists[0]);
    
    int result = max_length(lists, sizes, num_lists);
    printf("%d\n", result);
    
    return 0;
}