#include <stdio.h>
#include <stdlib.h>

int findMaxSum(int **triangle, int n) {
    if (triangle == NULL || n <= 0) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        if (triangle[i] == NULL) {
            return 0;
        }
    }
    int **dp = (int **)malloc(n * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        dp[i] = (int *)malloc((i + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
        }
    }
    dp[0][0] = triangle[0][0];
    for (int i = 1; i < n; i++) {
        dp[i][0] = dp[i - 1][0] + triangle[i][0];
        for (int j = 1; j < i; j++) {
            if (dp[i - 1][j - 1] > dp[i - 1][j]) {
                dp[i][j] = dp[i - 1][j - 1] + triangle[i][j];
            } else {
                dp[i][j] = dp[i - 1][j] + triangle[i][j];
            }
        }
        dp[i][i] = dp[i - 1][i - 1] + triangle[i][i];
    }
    int maxSum = dp[n - 1][0];
    for (int i = 1; i < n; i++) {
        if (dp[n - 1][i] > maxSum) {
            maxSum = dp[n - 1][i];
        }
    }
    for (int i = 0; i < n; i++) {
        free(dp[i]);
    }
    free(dp);
    return maxSum;
}

int main(void) {
    int n = 4;
    int **triangle = (int **)malloc(n * sizeof(int *));
    if (triangle == NULL) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        triangle[i] = (int *)malloc((i + 1) * sizeof(int));
        if (triangle[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(triangle[j]);
            }
            free(triangle);
            return EXIT_FAILURE;
        }
    }
    int r0[] = {1};
    int r1[] = {2, 3};
    int r2[] = {4, 5, 6};
    int r3[] = {7, 8, 9, 10};
    triangle[0][0] = r0[0];
    for (int i = 0; i < 2; i++) triangle[1][i] = r1[i];
    for (int i = 0; i < 3; i++) triangle[2][i] = r2[i];
    for (int i = 0; i < 4; i++) triangle[3][i] = r3[i];
    int maxSum = findMaxSum(triangle, n);
    printf("%d\n", maxSum);
    for (int i = 0; i < n; i++) {
        free(triangle[i]);
    }
    free(triangle);
    return EXIT_SUCCESS;
}