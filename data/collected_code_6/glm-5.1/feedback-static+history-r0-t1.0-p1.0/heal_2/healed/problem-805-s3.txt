#include <stdio.h>
#include <stdlib.h>

int* find_max_sum_list(int* const* lists, const int* sizes, int list_count, int* result_size) {
    if (lists == NULL || sizes == NULL || list_count <= 0) {
        *result_size = 0;
        return NULL;
    }

    long long max_sum = -9223372036854775807LL;
    int* result_list = NULL;
    *result_size = 0;

    for (int i = 0; i < list_count; i++) {
        if (lists[i] == NULL || sizes[i] <= 0) {
            continue;
        }

        long long current_sum = 0;
        for (int j = 0; j < sizes[i]; j++) {
            current_sum += lists[i][j];
        }

        if (current_sum > max_sum) {
            max_sum = current_sum;
            result_list = lists[i];
            *result_size = sizes[i];
        }
    }

    if (result_list == NULL) {
        *result_size = 0;
    }

    return result_list;
}

int main(void) {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5, 6, 7};
    int list3[] = {-1, -2, -3};
    int list4[] = {10};
    
    int* lists[] = {list1, list2, list3, list4};
    const int sizes[] = {3, 4, 3, 1};
    int list_count = 4;
    int result_size = 0;
    
    const int* result = find_max_sum_list(lists, sizes, list_count, &result_size);
    
    if (result != NULL && result_size > 0) {
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