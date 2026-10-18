#include <stdio.h>
#include <stdlib.h>

void remove_negatives(int **list, size_t *size) {
    if (list == NULL || *list == NULL || size == NULL || *size == 0) {
        return;
    }
    
    size_t write_index = 0;
    
    for (size_t i = 0; i < *size; i++) {
        if ((*list)[i] >= 0) {
            (*list)[write_index] = (*list)[i];
            write_index++;
        }
    }
    
    if (write_index < *size) {
        int *temp = realloc(*list, write_index * sizeof(int));
        if (temp == NULL && write_index > 0) {
            *size = write_index;
            return;
        }
        *list = temp;
    }
    
    *size = write_index;
}

int main(void) {
    size_t count = 10;
    int *numbers = malloc(count * sizeof(int));
    
    if (numbers == NULL) {
        return EXIT_FAILURE;
    }
    
    numbers[0] = 5;
    numbers[1] = -3;
    numbers[2] = 8;
    numbers[3] = -1;
    numbers[4] = 0;
    numbers[5] = -7;
    numbers[6] = 12;
    numbers[7] = -4;
    numbers[8] = 9;
    numbers[9] = -2;
    
    printf("Original array: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    remove_negatives(&numbers, &count);
    
    printf("Array after removing negatives: ");
    for (size_t i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    free(numbers);
    return EXIT_SUCCESS;
}