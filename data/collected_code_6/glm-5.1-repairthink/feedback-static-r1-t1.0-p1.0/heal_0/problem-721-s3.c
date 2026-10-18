#include <stdio.h>
#include <stdlib.h>

double max_avg_path(int **matrix, int n) {
    if (n <= 0) {
        return 0.0;
    }
    double **dp = (double **)malloc(n * sizeof(double *));
    if (dp == NULL) {
        return 0.0;
    }
    for (int i = 0; i < n; i++) {
        dp[i] = (double *)malloc(n * sizeof(double));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0.0;
        }
    }
    dp[0][0] = (double)matrix[0][0];
    for (int i = 1; i < n; i++) {
        dp[i][0] = dp[i - 1][0] + (double)matrix[i][0];
        dp[0][i] = dp[0][i - 1] + (double)matrix[0][i];
    }
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (dp[i - 1][j] > dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j] + (double)matrix[i][j];
            } else {
                dp[i][j] = dp[i][j - 1] + (double)matrix[i][j];
            }
        }
    }
    double max_avg = dp[n - 1][n - 1] / (2.0 * n - 1.0);
    for (int i = 0; i < n; i++) {
        free(dp[i]);
    }
    free(dp);
    return max_avg;
}

int main() {
    int n = 3;
    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return 1;
        }
    }
    matrix[0][0] = 1; matrix[0][1] = 2; matrix[0][2] = 3;
    matrix[1][0] = 4; matrix[1][1] = 5; matrix[1][2] = 6;
    matrix[2][0] = 7; matrix[2][1] = 8; matrix[2][2] = 9;
    double result = max_avg_path(matrix, n);
    printf("%f\n", result);
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}