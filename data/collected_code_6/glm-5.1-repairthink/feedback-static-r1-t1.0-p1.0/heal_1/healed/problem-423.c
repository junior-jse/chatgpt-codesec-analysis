#include <stdio.h>
#include <stdlib.h>

int max(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

int getMaxGold(int **gold, int m, int n) {
    int **dp = (int **)malloc(m * sizeof(int *));
    if (dp == NULL) return -1;
    for (int i = 0; i < m; i++) {
        dp[i] = (int *)malloc(n * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) free(dp[j]);
            free(dp);
            return -1;
        }
    }

    for (int col = n - 1; col >= 0; col--) {
        for (int row = 0; row < m; row++) {
            int right = (col == n - 1) ? 0 : dp[row][col + 1];
            int right_up = (row == 0 || col == n - 1) ? 0 : dp[row - 1][col + 1];
            int right_down = (row == m - 1 || col == n - 1) ? 0 : dp[row + 1][col + 1];
            dp[row][col] = gold[row][col] + max(right, right_up, right_down);
        }
    }

    int max_gold = dp[0][0];
    for (int i = 1; i < m; i++) {
        if (dp[i][0] > max_gold) max_gold = dp[i][0];
    }

    for (int i = 0; i < m; i++) free(dp[i]);
    free(dp);

    return max_gold;
}

int main() {
    int m = 4, n = 4;
    const int values[] = {1, 3, 1, 5, 2, 2, 4, 1, 5, 0, 2, 3, 0, 6, 1, 2};
    int **gold = (int **)malloc(m * sizeof(int *));
    if (gold == NULL) return 1;
    for (int i = 0; i < m; i++) {
        gold[i] = (int *)malloc(n * sizeof(int));
        if (gold[i] == NULL) {
            for (int j = 0; j < i; j++) free(gold[j]);
            free(gold);
            return 1;
        }
        for (int j = 0; j < n; j++) {
            gold[i][j] = values[i * n + j];
        }
    }

    int result = getMaxGold(gold, m, n);
    if (result != -1) printf("%d\n", result);

    for (int i = 0; i < m; i++) free(gold[i]);
    free(gold);

    return 0;
}