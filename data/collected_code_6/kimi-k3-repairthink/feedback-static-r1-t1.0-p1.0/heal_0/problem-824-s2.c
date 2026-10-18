#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

size_t remove_even_numbers(int *arr, size_t size)
{
    size_t i;
    size_t write_index = 0;

    if (arr == NULL && size > 0) {
        return 0;
    }

    for (i = 0; i < size; i++) {
        if (arr[i] % 2 != 0) {
            arr[write_index] = arr[i];
            write_index++;
        }
    }

    return write_index;
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);
    size_t new_size;
    size_t i;

    new_size = remove_even_numbers(numbers, size);

    for (i = 0; i < new_size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}