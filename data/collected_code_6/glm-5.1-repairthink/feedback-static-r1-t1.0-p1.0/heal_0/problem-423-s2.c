#include <stdio.h>
#include <stdlib.h>

int max(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = b;
    return m;
}

int getMaxGold(int **mine, int n, int m) {
    if (n == 0 || m == 0) return 0;

    int **dp = (int **)malloc(n * sizeof(int *));
    if (dp == NULL) return -1;

    for (int i = 0; i < n; i++) {
        dp[i] = (int *)malloc(m * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) free(dp[j]);
            free(dp);
            return -1;
        }
    }

    for (int i = 0; i < n; i++) {
        dp[i][m - 1] = mine[i][m - 1];
    }

    for (int col = m - 2; col >= 0; col--) {
        for (int row = 0; row < n; row++) {
            int right = dp[row][col + 1];
            int right_up = (row == 0) ? 0 : dp[row - 1][col + 1];
            int right_down = (row == n - 1) ? 0 : dp[row + 1][col + 1];
            dp[row][col] = mine[row][col] + max(right, right_up, right_down);
        }
    }

    int max_gold = dp[0][0];
    for (int i = 1; i < n; i++) {
        if (dp[i][0] > max_gold) {
            max_gold = dp[i][0];
        }
    }

    for (int i = 0; i < n; i++) free(dp[i]);
    free(dp);

    return max_gold;
}

int main() {
    int n = 4;
    int m = 4;

    int *mine_data[4] = {
        (int[]){1, 3, 1, 5},
        (int[]){2, 2, 4, 1},
        (int[]){5, 0, 2, 3},
        (int[]){0, 6, 1, 2}
    };

    int **mine = (int **)malloc(n * sizeof(int *));
    if (mine == NULL) return EXIT_FAILURE;

    for (int i = 0; i < n; i++) {
        mine[i] = (int *)malloc(m * sizeof(int));
        if (mine[i] == NULL) {
            for (int j = 0; j < i; j++) free(mine[j]);
            free(mine);
            return EXIT_FAILURE;
        }
        for (int j = 0; j < m; j++) {
            mine[i][j] = mine_data[i][j];
        }
    }

    int result = getMaxGold(mine, n, m);
    printf("%d\n", result);

    for (int i = 0; i < n; i++) free(mine[i]);
    free(mine);

    return 0;
}