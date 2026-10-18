#include <stdio.h>
#include <stdlib.h>

int **extract_elements(int **matrix, size_t rows, size_t cols, const size_t *indices, size_t num_indices, size_t *result_rows) {
    if (matrix == NULL || indices == NULL || result_rows == NULL || rows == 0 || cols == 0 || num_indices == 0) {
        return NULL;
    }
    
    for (size_t i = 0; i < num_indices; i++) {
        if (indices[i] >= cols) {
            return NULL;
        }
    }
    
    int **result = malloc(rows * sizeof(int *));
    if (result == NULL) {
        return NULL;
    }
    
    size_t allocated = 0;
    for (size_t i = 0; i < rows; i++) {
        result[i] = malloc(num_indices * sizeof(int));
        if (result[i] == NULL) {
            for (size_t j = 0; j < allocated; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        allocated++;
        
        for (size_t j = 0; j < num_indices; j++) {
            result[i][j] = matrix[i][indices[j]];
        }
    }
    
    *result_rows = rows;
    return result;
}

void free_matrix(int **matrix, size_t rows) {
    if (matrix == NULL) {
        return;
    }
    for (size_t i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(void) {
    size_t rows = 3;
    size_t cols = 4;
    
    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return EXIT_FAILURE;
        }
    }
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            matrix[i][j] = (int)(i * cols + j);
        }
    }
    
    /* Possible weaknesses found:
     *  Variable 'indices' can be declared as const array [constVariable]
     */
    size_t indices[] = {0, 2};
    size_t num_indices = 2;
    size_t result_rows = 0;
    
    int **result = extract_elements(matrix, rows, cols, indices, num_indices, &result_rows);
    
    if (result == NULL) {
        free_matrix(matrix, rows);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < result_rows; i++) {
        for (size_t j = 0; j < num_indices; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    free_matrix(result, result_rows);
    free_matrix(matrix, rows);
    
    return EXIT_SUCCESS;
}