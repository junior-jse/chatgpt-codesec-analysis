#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int **generate_spiral_matrix(int n) {
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
    int num = 1;
    int max_val = n * n;

    while (num <= max_val) {
        for (int i = left; i <= right && num <= max_val; i++) {
            matrix[top][i] = (int)pow(num, 2);
            num++;
        }
        top++;

        for (int i = top; i <= bottom && num <= max_val; i++) {
            matrix[i][right] = (int)pow(num, 2);
            num++;
        }
        right--;

        for (int i = right; i >= left && num <= max_val; i--) {
            matrix[bottom][i] = (int)pow(num, 2);
            num++;
        }
        bottom--;

        for (int i = bottom; i >= top && num <= max_val; i--) {
            matrix[i][left] = (int)pow(num, 2);
            num++;
        }
        left++;
    }

    return matrix;
}

void free_spiral_matrix(int **matrix, int n) {
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
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d", matrix[i][j]);
            if (j < n - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(void) {
    int n;
    printf("Enter matrix size: ");
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    if (n <= 0) {
        return 1;
    }

    int **matrix = generate_spiral_matrix(n);
    if (matrix == NULL) {
        return 1;
    }

    print_matrix(matrix, n);
    free_spiral_matrix(matrix, n);

    return 0;
}