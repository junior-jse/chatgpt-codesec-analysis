#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>

double find_max_avg_path_rec(int **matrix, int n, int i, int j, int current_sum, int current_length, double *max_avg) {
    if (i < 0 || i >= n || j < 0 || j >= n) {
        return *max_avg;
    }

    current_sum += matrix[i][j];
    current_length++;

    if (i == n - 1) {
        double avg = (double)current_sum / current_length;
        if (avg > *max_avg) {
            *max_avg = avg;
        }
        return *max_avg;
    }

    find_max_avg_path_rec(matrix, n, i + 1, j, current_sum, current_length, max_avg);
    find_max_avg_path_rec(matrix, n, i + 1, j + 1, current_sum, current_length, max_avg);
    find_max_avg_path_rec(matrix, n, i + 1, j - 1, current_sum, current_length, max_avg);

    return *max_avg;
}

double find_max_avg_path(int **matrix, int n) {
    if (matrix == NULL || n <= 0) {
        return 0.0;
    }

    double max_avg = -DBL_MAX;
    for (int j = 0; j < n; j++) {
        find_max_avg_path_rec(matrix, n, 0, j, 0, 0, &max_avg);
    }

    return max_avg;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid matrix size\n");
        return 1;
    }

    int **matrix = (int **)malloc((size_t)n * sizeof(int *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc((size_t)n * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Invalid matrix element\n");
                for (int k = 0; k < n; k++) {
                    free(matrix[k]);
                }
                free(matrix);
                return 1;
            }
        }
    }

    double result = find_max_avg_path(matrix, n);
    printf("%.6f\n", result);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}