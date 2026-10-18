#include <stdio.h>
#include <stdlib.h>

int **remove_column(int **matrix, int rows, int cols, int col_to_remove) {
    if (matrix == NULL || rows <= 0 || cols <= 1 || col_to_remove < 0 || col_to_remove >= cols) {
        return NULL;
    }
    
    int new_cols = cols - 1;
    int **new_matrix = (int **)malloc((size_t)rows * sizeof(int *));
    if (new_matrix == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        new_matrix[i] = (int *)malloc((size_t)new_cols * sizeof(int));
        if (new_matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(new_matrix[j]);
            }
            free(new_matrix);
            return NULL;
        }
        
        int src_col = 0;
        for (int j = 0; j < new_cols; j++) {
            if (src_col == col_to_remove) {
                src_col++;
            }
            new_matrix[i][j] = matrix[i][src_col];
            src_col++;
        }
    }
    
    return new_matrix;
}

void free_matrix(int **matrix, int rows) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(int **matrix, int rows, int cols) {
    if (matrix == NULL || rows <= 0 || cols <= 0) {
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows = 3;
    int cols = 4;
    int col_to_remove = 1;
    
    int **matrix = (int **)malloc((size_t)rows * sizeof(int *));
    if (matrix == NULL) {
        return 1;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc((size_t)cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return 1;
        }
    }
    
    int value = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value++;
        }
    }
    
    printf("Original matrix:\n");
    print_matrix(matrix, rows, cols);
    
    int **new_matrix = remove_column(matrix, rows, cols, col_to_remove);
    if (new_matrix == NULL) {
        free_matrix(matrix, rows);
        return 1;
    }
    
    printf("\nMatrix after removing column %d:\n", col_to_remove);
    print_matrix(new_matrix, rows, cols - 1);
    
    free_matrix(matrix, rows);
    free_matrix(new_matrix, rows);
    
    return 0;
}