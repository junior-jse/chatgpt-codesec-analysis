#include <stdio.h>
#include <stdlib.h>

int remove_evens(int *arr, size_t size)
{
    if (arr == NULL) {
        return -1;
    }

    size_t write_index = 0;
    for (size_t i = 0; i < size; i++) {
        if (arr[i] % 2 != 0) {
            arr[write_index] = arr[i];
            write_index++;
        }
    }
    return (int)write_index;
}

int main(void)
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int new_size;

    printf("Original array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    new_size = remove_evens(arr, size);
    if (new_size < 0) {
        fprintf(stderr, "Error: Invalid array\n");
        return EXIT_FAILURE;
    }

    printf("Array after removing evens: ");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}