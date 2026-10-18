#include <stdio.h>
#include <stdlib.h>

int** remove_column(int** matrix, int rows, int old_cols, int col_to_remove, int* new_cols) {
    if (matrix == NULL || rows <= 0 || old_cols <= 0 || new_cols == NULL || col_to_remove < 0 || col_to_remove >= old_cols) {
        if (new_cols != NULL) {
            *new_cols = 0;
        }
        return NULL;
    }

    *new_cols = old_cols - 1;

    if (*new_cols == 0) {
        for (int i = 0; i < rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
        return NULL;
    }

    int** new_matrix = (int**)malloc(rows * sizeof(int*));
    if (new_matrix == NULL) {
        *new_cols = 0;
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        new_matrix[i] = (int*)malloc(*new_cols * sizeof(int));
        if (new_matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(new_matrix[j]);
            }
            free(new_matrix);
            *new_cols = 0;
            return NULL;
        }

        int current_col = 0;
        for (int j = 0; j < old_cols; j++) {
            if (j != col_to_remove) {
                new_matrix[i][current_col++] = matrix[i][j];
            }
        }
        free(matrix[i]);
    }
    
    free(matrix);
    return new_matrix;
}

int** create_matrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) return NULL;

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    return matrix;
}

int main() {
    int rows = 4;
    int cols = 5;
    int col_to_remove = 2;
    int new_cols = 0;

    int** matrix = create_matrix(rows, cols);
    if (matrix == NULL) {
        return 1;
    }

    matrix = remove_column(matrix, rows, cols, col_to_remove, &new_cols);
    if (matrix != NULL) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < new_cols; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
            free(matrix[i]);
        }
        free(matrix);
    }

    return 0;
}