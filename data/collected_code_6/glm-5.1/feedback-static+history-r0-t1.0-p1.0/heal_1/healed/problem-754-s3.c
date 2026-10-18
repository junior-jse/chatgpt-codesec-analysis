#include <stdio.h>
#include <stdlib.h>

int* find_common_index_elements(const int* arr1, const int* arr2, const int* arr3, int size1, int size2, int size3, int* result_size) {
    if (arr1 == NULL || arr2 == NULL || arr3 == NULL || result_size == NULL) {
        return NULL;
    }

    if (size1 < 0 || size2 < 0 || size3 < 0) {
        return NULL;
    }

    int min_size = size1;
    if (size2 < min_size) min_size = size2;
    if (size3 < min_size) min_size = size3;

    if (min_size == 0) {
        *result_size = 0;
        return NULL;
    }

    int* result = (int*)malloc(min_size * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < min_size; i++) {
        if (arr1[i] == arr2[i] && arr2[i] == arr3[i]) {
            result[count] = arr1[i];
            count++;
        }
    }

    if (count == 0) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    int* temp = (int*)realloc(result, count * sizeof(int));
    if (temp != NULL) {
        result = temp;
    }

    *result_size = count;
    return result;
}

int main() {
    int arr1[] = {1, 5, 10, 20, 40, 80};
    int arr2[] = {6, 7, 20, 80, 100};
    int arr3[] = {3, 4, 15, 20, 30, 70, 80, 120};

    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int size3 = sizeof(arr3) / sizeof(arr3[0]);

    int result_size = 0;
    int* result = find_common_index_elements(arr1, arr2, arr3, size1, size2, size3, &result_size);

    if (result != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No common elements at the same indices found.\n");
    }

    return 0;
}