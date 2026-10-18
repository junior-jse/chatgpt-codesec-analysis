#include <stdio.h>
#include <stdlib.h>

int remove_odd(int *array, size_t size) {
    if (array == NULL) {
        return 0;
    }

    size_t even_index = 0;

    for (size_t i = 0; i < size; ++i) {
        if (array[i] % 2 == 0) {
            array[even_index] = array[i];
            ++even_index;
        }
    }

    return (int)even_index;
}

int main() {
    size_t capacity = 10;
    int *numbers = malloc(capacity * sizeof(int));
    
    if (numbers == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < capacity; ++i) {
        numbers[i] = (int)i + 1;
    }

    int new_size = remove_odd(numbers, capacity);

    for (int i = 0; i < new_size; ++i) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    free(numbers);
    numbers = NULL;

    return EXIT_SUCCESS;
}