#include <stdio.h>
#include <stdlib.h>

void reverse(int *arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int rotate_right(int *arr, int size, int k) {
    if (arr == NULL || size <= 0 || k < 0) {
        return -1;
    }
    
    k = k % size;
    if (k == 0) {
        return 0;
    }
    
    reverse(arr, 0, size - 1);
    reverse(arr, 0, k - 1);
    reverse(arr, k, size - 1);
    
    return 0;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    int k = 3;
    
    printf("Original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    if (rotate_right(arr, size, k) == 0) {
        printf("Rotated array: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Error: Invalid input parameters\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}