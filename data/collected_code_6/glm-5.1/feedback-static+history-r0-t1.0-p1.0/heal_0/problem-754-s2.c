#include <stdio.h>
#include <stdlib.h>

int* find_common_index_elements(int* arr1, int* arr2, int* arr3, int size1, int size2, int size3, int* result_size) {
    int min_size = size1;
    if (size2 < min_size) min_size = size2;
    if (size3 < min_size) min_size = size3;

    if (min_size <= 0) {
        *result_size = 0;
        return NULL;
    }

    int* result = (int*)malloc(min_size * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < min_size; i++) {
        if (arr1[i] == arr2[i] && arr2[i] == arr3[i]) {
            result[count] = i;
            count++;
        }
    }

    if (count == 0) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    int* final_result = (int*)realloc(result, count * sizeof(int));
    if (final_result == NULL) {
        *result_size = count;
        return result;
    }

    *result_size = count;
    return final_result;
}

int main(void) {
    int arr1[] = {1, 5, 3, 8, 9};
    int arr2[] = {4, 5, 3, 7, 2};
    int arr3[] = {9, 5, 3, 6, 1};

    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    int result_size = 0;

    int* common_indices = find_common_index_elements(arr1, arr2, arr3, size1, size2, size3, &result_size);

    if (common_indices != NULL && result_size > 0) {
        for (int i = 0; i < result_size; i++) {
            printf("%d ", common_indices[i]);
        }
        printf("\n");
        free(common_indices);
    } else {
        printf("No common index elements found.\n");
    }

    return 0;
}