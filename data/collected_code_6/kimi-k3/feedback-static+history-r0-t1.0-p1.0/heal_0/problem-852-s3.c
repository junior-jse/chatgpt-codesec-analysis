#include <stdio.h>
#include <stdlib.h>

int remove_negatives(int *arr, size_t size) {
    if (arr == NULL) {
        return -1;
    }
    
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < size; read_idx++) {
        if (arr[read_idx] >= 0) {
            arr[write_idx] = arr[read_idx];
            write_idx++;
        }
    }
    
    return (int)write_idx;
}

int main(void) {
    int arr[] = {5, -3, 8, -1, 0, -7, 12, -4, 9};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    
    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    int new_size = remove_negatives(arr, size);
    
    if (new_size < 0) {
        fprintf(stderr, "Error: Invalid array\n");
        return EXIT_FAILURE;
    }
    
    printf("Array after removing negatives: ");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}