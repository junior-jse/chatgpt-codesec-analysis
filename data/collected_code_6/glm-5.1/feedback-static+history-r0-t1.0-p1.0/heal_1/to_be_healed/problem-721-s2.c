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

    int **len = (int **)malloc(n * sizeof(int *));
    if (len == NULL) {
        for (int i = 0; i < n; i++) {
            free(dp[i]);
        }
        free(dp);
        return 0.0;
    }
    for (int i = 0; i < n; i++) {
        len[i] = (int *)malloc(n * sizeof(int));
        if (len[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(len[j]);
            }
            free(len);
            for (int j = 0; j < n; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0.0;
        }
    }

    dp[0][0] = matrix[0][0];
    len[0][0] = 1;

    for (int i = 1; i < n; i++) {
        dp[i][0] = dp[i - 1][0] + matrix[i][0];
        len[i][0] = len[i - 1][0] + 1;
        dp[0][i] = dp[0][i - 1] + matrix[0][i];
        len[0][i] = len[0][i - 1] + 1;
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            double avg_up = (dp[i - 1][j] + matrix[i][j]) / (len[i - 1][j] + 1);
            double avg_left = (dp[i][j - 1] + matrix[i][j]) / (len[i][j - 1] + 1);
            if (avg_up > avg_left) {
                dp[i][j] = dp[i - 1][j] + matrix[i][j];
                len[i][j] = len[i - 1][j] + 1;
            } else {
                dp[i][j] = dp[i][j - 1] + matrix[i][j];
                /* Possible weaknesses found:
                 *  expected ';' after expression
                 *  expected ';' before numeric constant
                 */
                len[i][j] = len[i][j - 1]1;
            }
        }
    }

    double result = dp[n - 1][n - 1] / len[n - 1][n - 1];

    for (int i = 0; i < n; i++) {
        free(dp[i]);
        free(len[i]);
    }
    free(dp);
    free(len);

    return result;
}

int main() {
    int n = 3;
    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return EXIT_FAILURE;
        }
    }

    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    double max_avg = max_avg_path(matrix, n);
    printf("%f\n", max_avg);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return EXIT_SUCCESS;
}