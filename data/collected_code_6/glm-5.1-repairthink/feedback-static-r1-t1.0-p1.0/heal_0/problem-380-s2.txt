#include <stdio.h>
#include <stdlib.h>

int** generate_2d_array(size_t rows, size_t cols) {
    int **array = (int **)malloc(rows * sizeof(int *));
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < rows; i++) {
        array[i] = (int *)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            array[i][j] = (int)(i * cols + j);
        }
    }

    return array;
}

void free_2d_array(int **array, size_t rows) {
    if (array == NULL) {
        return;
    }
    for (size_t i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

int main(void) {
    size_t rows = 4;
    size_t cols = 5;

    int **array = generate_2d_array(rows, cols);
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }

    free_2d_array(array, rows);

    return 0;
}