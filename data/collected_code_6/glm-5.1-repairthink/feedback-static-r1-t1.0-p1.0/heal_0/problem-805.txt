#include <stdio.h>
#include <stdlib.h>

int* find_max_sum_list(int** lists, int* lengths, int list_count, int* result_length) {
    if (lists == NULL || lengths == NULL || list_count <= 0) {
        *result_length = 0;
        return NULL;
    }

    long long max_sum = -1;
    int max_index = -1;

    for (int i = 0; i < list_count; ++i) {
        if (lengths[i] > 0 && lists[i] != NULL) {
            long long current_sum = 0;
            for (int j = 0; j < lengths[i]; ++j) {
                current_sum += lists[i][j];
            }
            if (current_sum > max_sum) {
                max_sum = current_sum;
                max_index = i;
            }
        }
    }

    if (max_index == -1) {
        *result_length = 0;
        return NULL;
    }

    *result_length = lengths[max_index];
    return lists[max_index];
}

int main(void) {
    int list1[] = {1, 2, 3};
    int list2[] = {10, 20};
    int list3[] = {-5, 5, 15, 10};
    int list4[] = {};

    int* lists[] = {list1, list2, list3, list4};
    int lengths[] = {3, 2, 4, 0};
    int list_count = 4;
    int result_length = 0;

    int* result = find_max_sum_list(lists, lengths, list_count, &result_length);

    if (result != NULL && result_length > 0) {
        printf("List with highest sum: ");
        for (int i = 0; i < result_length; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");
    } else {
        printf("No valid list found.\n");
    }

    return 0;
}