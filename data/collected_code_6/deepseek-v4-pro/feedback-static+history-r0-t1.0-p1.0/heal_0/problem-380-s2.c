#include <stdio.h>
#include <stdlib.h>

int **generate_array(int rows, int cols) {
    int **arr;
    int i, j;

    if (rows <= 0 || cols <= 0) {
        return NULL;
    }

    arr = (int **)malloc((size_t)rows * sizeof(int *));
    if (arr == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        arr[i] = (int *)malloc((size_t)cols * sizeof(int));
        if (arr[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            return NULL;
        }
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            arr[i][j] = i * cols + j;
        }
    }

    return arr;
}

void free_array(int **arr, int rows) {
    int i;

    if (arr == NULL) {
        return;
    }

    for (i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}

int main(void) {
    int **matrix;
    int rows = 3;
    int cols = 4;
    int i, j;

    matrix = generate_array(rows, cols);
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    free_array(matrix, rows);

    return EXIT_SUCCESS;
}