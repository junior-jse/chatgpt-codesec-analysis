#include <stdio.h>
#include <stdlib.h>

int findCommonElements(int *arr1, int size1, int *arr2, int size2, int *arr3, int size3, int *result, int maxResultSize) {
    if (arr1 == NULL || arr2 == NULL || arr3 == NULL || result == NULL) {
        return -1;
    }
    
    if (size1 <= 0 || size2 <= 0 || size3 <= 0 || maxResultSize <= 0) {
        return -1;
    }
    
    int count = 0;
    int minSize = size1;
    
    if (size2 < minSize) {
        minSize = size2;
    }
    if (size3 < minSize) {
        minSize = size3;
    }
    
    for (int i = 0; i < minSize && count < maxResultSize; i++) {
        if (arr1[i] == arr2[i] && arr2[i] == arr3[i]) {
            result[count] = arr1[i];
            count++;
        }
    }
    
    return count;
}

int main(void) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 6, 5};
    int arr3[] = {1, 2, 3, 7, 5};
    
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    int minSize = size1;
    if (size2 < minSize) {
        minSize = size2;
    }
    if (size3 < minSize) {
        minSize = size3;
    }
    
    int *result = (int *)malloc(minSize * sizeof(int));
    
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    int commonCount = findCommonElements(arr1, size1, arr2, size2, arr3, size3, result, minSize);
    
    if (commonCount < 0) {
        fprintf(stderr, "Error in finding common elements\n");
        free(result);
        return EXIT_FAILURE;
    }
    
    if (commonCount == 0) {
        printf("No common elements found at the same indices\n");
    } else {
        printf("Common elements at the same indices: ");
        for (int i = 0; i < commonCount; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }
    
    free(result);
    result = NULL;
    
    return EXIT_SUCCESS;
}