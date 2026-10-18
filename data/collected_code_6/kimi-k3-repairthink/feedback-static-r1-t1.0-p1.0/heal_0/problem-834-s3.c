#include <stdio.h>
#include <stdlib.h>

int **generateSpiralMatrix(int n) {
    if (n <= 0) {
        return NULL;
    }

    int **matrix = (int **)malloc((size_t)n * sizeof(int *));
    if (matrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc((size_t)n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    int num = 1;
    int top = 0, bottom = n - 1;
    int left = 0, right = n - 1;

    while (num <= n * n) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = num++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = num++;
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = num++;
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = num++;
            }
            left++;
        }
    }

    return matrix;
}

void freeMatrix(int **matrix, int n) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(int **matrix, int n) {
    if (matrix == NULL || n <= 0) {
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n = 0;

    printf("Enter the size of the matrix (n): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }

    if (n <= 0) {
        fprintf(stderr, "Error: Size must be positive\n");
        return EXIT_FAILURE;
    }

    int **matrix = generateSpiralMatrix(n);
    if (matrix == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Spiral Matrix:\n");
    printMatrix(matrix, n);

    freeMatrix(matrix, n);

    return EXIT_SUCCESS;
}