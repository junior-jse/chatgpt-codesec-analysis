#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

double findMaxAveragePath(int **matrix, int n) {
    if (matrix == NULL || n <= 0) {
        return 0.0;
    }

    double **maxSum = (double **)malloc(n * sizeof(double *));
    int **pathLength = (int **)malloc(n * sizeof(int *));
    if (maxSum == NULL || pathLength == NULL) {
        if (maxSum != NULL) free(maxSum);
        if (pathLength != NULL) free(pathLength);
        return 0.0;
    }

    for (int i = 0; i < n; i++) {
        maxSum[i] = (double *)malloc(n * sizeof(double));
        pathLength[i] = (int *)malloc(n * sizeof(int));
        if (maxSum[i] == NULL || pathLength[i] == NULL) {
            for (int j = 0; j <= i; j++) {
                if (maxSum[j] != NULL) free(maxSum[j]);
                if (pathLength[j] != NULL) free(pathLength[j]);
            }
            free(maxSum);
            free(pathLength);
            return 0.0;
        }
    }

    maxSum[0][0] = matrix[0][0];
    pathLength[0][0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) continue;

            double bestSum = -1.0;
            int bestLen = 0;

            if (i > 0) {
                double avg = maxSum[i-1][j] / pathLength[i-1][j];
                if (avg > bestSum || (avg == bestSum && pathLength[i-1][j] > bestLen)) {
                    bestSum = avg;
                    bestLen = pathLength[i-1][j];
                }
            }

            if (j > 0) {
                double avg = maxSum[i][j-1] / pathLength[i][j-1];
                if (avg > bestSum || (avg == bestSum && pathLength[i][j-1] > bestLen)) {
                    bestSum = avg;
                    bestLen = pathLength[i][j-1];
                }
            }

            pathLength[i][j] = bestLen + 1;
            maxSum[i][j] = bestSum * bestLen + matrix[i][j];
        }
    }

    double result = maxSum[n-1][n-1] / pathLength[n-1][n-1];

    for (int i = 0; i < n; i++) {
        free(maxSum[i]);
        free(pathLength[i]);
    }
    free(maxSum);
    free(pathLength);

    return result;
}

int main() {
    int n;
    printf("Enter matrix size: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid matrix size\n");
        return 1;
    }

    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            printf("Memory allocation failed\n");
            return 1;
        }
    }

    printf("Enter matrix elements:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    double maxAvg = findMaxAveragePath(matrix, n);
    printf("Maximum average path: %f\n", maxAvg);

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}