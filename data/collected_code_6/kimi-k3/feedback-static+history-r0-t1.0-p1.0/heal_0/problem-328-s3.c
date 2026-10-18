#include <stdio.h>
#include <stdlib.h>

void rotate_left(int *arr, size_t size, size_t positions) {
    if (arr == NULL || size == 0) {
        return;
    }
    
    positions = positions % size;
    if (positions == 0) {
        return;
    }
    
    int *temp = (int *)malloc(positions * sizeof(int));
    if (temp == NULL) {
        return;
    }
    
    for (size_t i = 0; i < positions; i++) {
        temp[i] = arr[i];
    }
    
    for (size_t i = 0; i < size - positions; i++) {
        arr[i] = arr[i + positions];
    }
    
    for (size_t i = 0; i < positions; i++) {
        arr[size - positions + i] = temp[i];
    }
    
    free(temp);
    temp = NULL;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t positions = 2;
    
    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    rotate_left(arr, size, positions);
    
    printf("Rotated array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}