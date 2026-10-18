#include <stdio.h>
#include <stdlib.h>

int** generate_spiral_matrix(int n) {
    if (n <= 0) {
        return NULL;
    }

    int** matrix = (int**)malloc(n * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int current = 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] = current * current;
            current++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            matrix[i][right] = current * current;
            current++;
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = current * current;
                current++;
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = current * current;
                current++;
            }
            left++;
        }
    }

    return matrix;
}

void free_matrix(int** matrix, int n) {
    if (matrix == NULL || n <= 0) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char *argv[]) {
    int n = 3;

    if (argc == 2) {
        n = atoi(argv[1]);
        if (n <= 0) {
            fprintf(stderr, "Invalid matrix size\n");
            return 1;
        }
    }

    int** matrix = generate_spiral_matrix(n);
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
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