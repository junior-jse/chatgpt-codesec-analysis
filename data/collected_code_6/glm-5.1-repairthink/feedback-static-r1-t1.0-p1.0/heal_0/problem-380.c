#include <stdio.h>
#include <stdlib.h>

int** generate_2d_array(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        return NULL;
    }

    int** array = (int**)malloc(rows * sizeof(int*));
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < rows; i++) {
        array[i] = (int*)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
        for (size_t k = 0; k < cols; k++) {
            array[i][k] = 0;
        }
    }

    return array;
}

void free_2d_array(int** array, size_t rows) {
    if (array == NULL) {
        return;
    }
    for (size_t i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

int main(void) {
    size_t rows = 5;
    size_t cols = 10;

    int** arr = generate_2d_array(rows, cols);
    if (arr == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            arr[i][j] = (int)(i * cols + j);
        }
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    free_2d_array(arr, rows);

    return EXIT_SUCCESS;
}