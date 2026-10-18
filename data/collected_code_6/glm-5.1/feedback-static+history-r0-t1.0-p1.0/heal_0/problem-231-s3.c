#include <stdio.h>
#include <stdlib.h>

int findMaxSum(int **triangle, int n) {
    if (n <= 0) {
        return 0;
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

int main() {
    int n = 4;
    int row0[] = {2};
    int row1[] = {5, 4};
    int row2[] = {1, 4, 7};
    int row3[] = {8, 6, 9, 1};

    int **triangle = (int **)malloc(n * sizeof(int *));
    if (triangle == NULL) {
        return 1;
    }

    triangle[0] = row0;
    triangle[1] = row1;
    triangle[2] = row2;
    triangle[3] = row3;

    int result = findMaxSum(triangle, n);
    printf("%d\n", result);

    free(triangle);

    return 0;
}