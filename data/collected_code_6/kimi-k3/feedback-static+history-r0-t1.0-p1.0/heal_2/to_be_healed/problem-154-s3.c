#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **extract_elements(int **matrix, int rows, int cols, const int *indices, int indices_count, int *result_rows) {
    if (matrix == NULL || indices == NULL || rows <= 0 || cols <= 0 || indices_count <= 0 || result_rows == NULL) {
        return NULL;
    }
    
    int **result = (int **)malloc((size_t)rows * sizeof(int *));
    if (result == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        result[i] = (int *)malloc((size_t)indices_count * sizeof(int));
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        for (int j = 0; j < indices_count; j++) {
            if (indices[j] >= 0 && indices[j] < cols) {
                result[i][j] = matrix[i][indices[j]];
            } else {
                result[i][j] = 0;
            }
        }
    }
    
    *result_rows = rows;
    return result;
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

int main(void) {
    int rows = 3;
    int cols = 4;
    
    int **matrix = (int **)malloc((size_t)rows * sizeof(int *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc((size_t)cols * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return EXIT_FAILURE;
        }
    }
    
    int value = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = value++;
        }
    }
    
    /* Possible weaknesses found:
     *  Variable 'indices' can be declared as const array [constVariable]
     */
    int indices[] = {0, 2};
    int indices_count = 2;
    int result_rows = 0;
    
    int **result = extract_elements(matrix, rows, cols, indices, indices_count, &result_rows);
    if (result == NULL) {
        fprintf(stderr, "Extraction failed\n");
        free_matrix(matrix, rows);
        return EXIT_FAILURE;
    }
    
    printf("Original matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    printf("\nExtracted elements:\n");
    for (int i = 0; i < result_rows; i++) {
        for (int j = 0; j < indices_count; j++) {
            printf("%3d ", result[i][j]);
        }
        printf("\n");
    }
    
    free_matrix(matrix, rows);
    free_matrix(result, result_rows);
    
    return EXIT_SUCCESS;
}