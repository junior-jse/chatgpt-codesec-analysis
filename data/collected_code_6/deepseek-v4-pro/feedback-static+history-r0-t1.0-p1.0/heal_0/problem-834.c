#include <stdio.h>
#include <stdlib.h>

int **generate_matrix(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    
    int top = 0, bottom = n - 1;
    int left = 0, right = n - 1;
    int value = 1;
    
    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = value * value;
            value++;
        }
        top++;
        
        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = value * value;
            value++;
        }
        right--;
        
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = value * value;
                value++;
            }
            bottom--;
        }
        
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = value * value;
                value++;
            }
            left++;
        }
    }
    
    return matrix;
}

void free_matrix(int **matrix, int n) {
    if (matrix == NULL) {
        return;
    }
    
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(int **matrix, int n) {
    if (matrix == NULL) {
        printf("NULL matrix\n");
        return;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n = 4;
    int **matrix = generate_matrix(n);
    
    if (matrix == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    
    printf("Spiral matrix of size %d:\n", n);
    print_matrix(matrix, n);
    
    free_matrix(matrix, n);
    
    return 0;
}