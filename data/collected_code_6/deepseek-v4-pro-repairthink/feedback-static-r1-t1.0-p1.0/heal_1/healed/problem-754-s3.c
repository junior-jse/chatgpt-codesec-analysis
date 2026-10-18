#include <stdio.h>
#include <stdlib.h>

int find_common_indices(const int *arr1, int size1, const int *arr2, int size2, const int *arr3, int size3, int *result) {
    if (arr1 == NULL || arr2 == NULL || arr3 == NULL || result == NULL) {
        return -1;
    }
    
    if (size1 <= 0 || size2 <= 0 || size3 <= 0) {
        return 0;
    }
    
    int min_size = size1;
    if (size2 < min_size) min_size = size2;
    if (size3 < min_size) min_size = size3;
    
    int count = 0;
    for (int i = 0; i < min_size; i++) {
        if (arr1[i] == arr2[i] && arr2[i] == arr3[i]) {
            result[count++] = i;
        }
    }
    
    return count;
}

int main(void) {
    int arr1[] = {1, 2, 3, 4, 5, 6};
    int arr2[] = {1, 2, 0, 4, 5, 7};
    int arr3[] = {1, 9, 3, 4, 8, 6};
    
    int result[6];
    
    int count = find_common_indices(arr1, 6, arr2, 6, arr3, 6, result);
    
    if (count < 0) {
        printf("Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Common indices: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    int arr4[] = {5, 10, 15};
    int arr5[] = {5, 10, 15};
    int arr6[] = {5, 10, 15};
    int result2[3];
    
    count = find_common_indices(arr4, 3, arr5, 3, arr6, 3, result2);
    
    if (count < 0) {
        printf("Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Common indices: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", result2[i]);
    }
    printf("\n");
    
    int arr7[] = {1, 2, 3};
    int arr8[] = {4, 5, 6};
    int arr9[] = {7, 8, 9};
    int result3[3];
    
    count = find_common_indices(arr7, 3, arr8, 3, arr9, 3, result3);
    
    if (count < 0) {
        printf("Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Common indices: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", result3[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}