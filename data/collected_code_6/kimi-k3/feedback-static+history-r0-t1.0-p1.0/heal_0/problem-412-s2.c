#include <stdio.h>
#include <stdlib.h>

size_t remove_odd_numbers(int *numbers, size_t size)
{
    size_t i;
    size_t write_index;

    if (numbers == NULL) {
        return 0;
    }

    write_index = 0;
    for (i = 0; i < size; ++i) {
        if (numbers[i] % 2 == 0) {
            numbers[write_index] = numbers[i];
            ++write_index;
        }
    }

    return write_index;
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t size;
    size_t new_size;
    size_t i;

    size = sizeof(numbers) / sizeof(numbers[0]);
    new_size = remove_odd_numbers(numbers, size);

    printf("List after removing odd numbers: ");
    for (i = 0; i < new_size; ++i) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}