#include <stdio.h>
#include <stdlib.h>

int** remove_column(int** matrix, int rows, int cols, int col_index, int** out_sizes) {
    if (matrix == NULL || rows <= 0 || cols <= 0 || col_index < 0 || col_index >= cols) {
        if (out_sizes) *out_sizes = NULL;
        return NULL;
    }

    int* sizes = (int*)malloc(rows * sizeof(int));
    if (sizes == NULL) {
        if (out_sizes) *out_sizes = NULL;
        return NULL;
    }

    int** result = (int**)malloc(rows * sizeof(int*));
    if (result == NULL) {
        free(sizes);
        if (out_sizes) *out_sizes = NULL;
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        if (matrix[i] == NULL) {
            result[i] = NULL;
            sizes[i] = 0;
            continue;
        }

        int new_cols = cols - 1;
        sizes[i] = new_cols;
        
        result[i] = (int*)malloc(new_cols * sizeof(int));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            free(sizes);
            if (out_sizes) *out_sizes = NULL;
            return NULL;
        }

        int r_idx = 0;
        for (int j = 0; j < cols; j++) {
            if (j != col_index) {
                result[i][r_idx++] = matrix[i][j];
            }
        }
    }

    *out_sizes = sizes;
    return result;
}

int main() {
    int rows = 3;
    int cols = 4;
    int col_to_remove = 2;

    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) return 1;

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return 1;
        }
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }

    int* new_sizes = NULL;
    int** new_matrix = remove_column(matrix, rows, cols, col_to_remove, &new_sizes);

    if (new_matrix != NULL && new_sizes != NULL) {
        for (int i = 0; i < rows; i++) {
            if (new_matrix[i] != NULL) {
                for (int j = 0; j < new_sizes[i]; j++) {
                    printf("%d ", new_matrix[i][j]);
                }
                printf("\n");
            }
        }
    }

    if (new_matrix != NULL) {
        for (int i = 0; i < rows; i++) {
            free(new_matrix[i]);
        }
        free(new_matrix);
        free(new_sizes);
    }

    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}