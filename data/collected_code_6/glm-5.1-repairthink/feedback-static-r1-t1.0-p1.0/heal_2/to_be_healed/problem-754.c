#include <stdio.h>
#include <stdlib.h>

int* find_common_index_elements(const int* a, const int* b, const int* c, int size, int* result_size) {
    if (a == NULL || b == NULL || c == NULL || size <= 0 || result_size == NULL) {
        if (result_size != NULL) {
            *result_size = 0;
        }
        return NULL;
    }

    int* result = (int*)malloc(size * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < size; i++) {
        if (a[i] && b[i] && c[i]) {
            result[count++] = i;
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
    int arr1[] = {0, 1, 1, 0, 1};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {0, 1, 0, 1, 1};
    /* Possible weaknesses found:
     *  Variable 'arr3' can be declared as const array [constVariable]
     */
    int arr3[] = {1, 1, 0, 0, 1};
    int size = sizeof(arr1) / sizeof(arr1[0]);
    int result_size = 0;

    int* common_indices = find_common_index_elements(arr1, arr2, arr3, size, &result_size);

    if (common_indices != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("%d\n", common_indices[i]);
        }
        free(common_indices);
    }

    return 0;
}