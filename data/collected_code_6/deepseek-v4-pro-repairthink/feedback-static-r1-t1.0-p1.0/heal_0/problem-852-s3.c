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
        size_t new_size = write_index;
        int *temp = (int *)realloc(*list, new_size * sizeof(int));
        if (temp == NULL && new_size > 0) {
            return;
        }
        *list = temp;
        *size = write_index;
    }
}

int main(void) {
    size_t size = 8;
    int *numbers = (int *)malloc(size * sizeof(int));
    if (numbers == NULL) {
        return 1;
    }
    
    numbers[0] = 5;
    numbers[1] = -3;
    numbers[2] = 8;
    numbers[3] = -1;
    numbers[4] = 0;
    numbers[5] = 12;
    numbers[6] = -7;
    numbers[7] = 4;
    
    remove_negatives(&numbers, &size);
    
    for (size_t i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    free(numbers);
    return 0;
}