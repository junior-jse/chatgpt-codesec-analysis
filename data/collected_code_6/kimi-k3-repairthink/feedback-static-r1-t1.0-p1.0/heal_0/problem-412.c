#include <stdio.h>
#include <stdlib.h>

size_t remove_odd_numbers(int *arr, size_t size) {
    if (arr == NULL) {
        return 0;
    }
    
    size_t write_index = 0;
    
    for (size_t read_index = 0; read_index < size; read_index++) {
        if (arr[read_index] % 2 == 0) {
            arr[write_index] = arr[read_index];
            write_index++;
        }
    }
    
    return write_index;
}

int main(void) {
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    size_t new_size = remove_odd_numbers(numbers, size);
    
    printf("Array after removing odd numbers: ");
    for (size_t i = 0; i < new_size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    return 0;
}