#include <stdio.h>
#include <stdlib.h>

int** generate_spiral_matrix(int n) {
    if (n <= 0) {
        return NULL;
    }

    int** matrix = (int**)malloc(n * sizeof(int*));
    if (!matrix) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    int top = 0;
    int bottom = n - 1;
    int left = 0;
    int right = n - 1;
    int val = 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = val * val;
            val++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = val * val;
            val++;
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = val * val;
                val++;
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = val * val;
                val++;
            }
            left++;
        }
    }

    return matrix;
}

void free_matrix(int** matrix, int n) {
    if (!matrix) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int n = 3;
    int** matrix = generate_spiral_matrix(n);

    if (!matrix) {
        fprintf(stderr, "Memory allocation failed or invalid n.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    free_matrix(matrix, n);
    return 0;
}