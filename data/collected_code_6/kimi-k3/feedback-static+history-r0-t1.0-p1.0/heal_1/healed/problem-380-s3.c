#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** generate_2d_array(size_t rows, size_t cols, int init_value) {
    int** array = NULL;
    size_t i = 0;
    
    if (rows == 0 || cols == 0) {
        return NULL;
    }
    
    array = (int**)malloc(rows * sizeof(int*));
    if (array == NULL) {
        return NULL;
    }
    
    for (i = 0; i < rows; i++) {
        array[i] = NULL;
    }
    
    for (i = 0; i < rows; i++) {
        size_t j = 0;
        array[i] = (int*)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            size_t k = 0;
            for (k = 0; k < i; k++) {
                free(array[k]);
            }
            free(array);
            return NULL;
        }
        
        for (j = 0; j < cols; j++) {
            array[i][j] = init_value;
        }
    }
    
    return array;
}

void free_2d_array(int** array, size_t rows) {
    size_t i = 0;
    
    if (array == NULL) {
        return;
    }
    
    for (i = 0; i < rows; i++) {
        if (array[i] != NULL) {
            free(array[i]);
        }
    }
    free(array);
}

int main(void) {
    size_t rows = 5;
    size_t cols = 4;
    int** array = NULL;
    size_t i = 0;
    
    array = generate_2d_array(rows, cols, 0);
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < rows; i++) {
        size_t j = 0;
        for (j = 0; j < cols; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    
    free_2d_array(array, rows);
    
    return EXIT_SUCCESS;
}