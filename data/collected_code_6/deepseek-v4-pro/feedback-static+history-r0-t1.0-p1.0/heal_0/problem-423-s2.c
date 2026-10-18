#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int collect_gold(int **mine, int rows, int cols) {
    if (mine == NULL || rows <= 0 || cols <= 0) {
        return 0;
    }

    int **dp = (int **)malloc(rows * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }

    for (int i = 0; i < rows; i++) {
        dp[i] = (int *)malloc(cols * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
        }
    }

    for (int i = 0; i < rows; i++) {
        dp[i][cols - 1] = mine[i][cols - 1];
    }

    for (int col = cols - 2; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            int right = dp[row][col + 1];
            int right_up = (row > 0) ? dp[row - 1][col + 1] : 0;
            int right_down = (row < rows - 1) ? dp[row + 1][col + 1] : 0;

            int max_right = right;
            if (right_up > max_right) {
                max_right = right_up;
            }
            if (right_down > max_right) {
                max_right = right_down;
            }

            dp[row][col] = mine[row][col] + max_right;
        }
    }

    int max_gold = dp[0][0];
    for (int i = 1; i < rows; i++) {
        if (dp[i][0] > max_gold) {
            max_gold = dp[i][0];
        }
    }

    for (int i = 0; i < rows; i++) {
        free(dp[i]);
    }
    free(dp);

    return max_gold;
}

int **allocate_mine(int rows, int cols) {
    int **mine = (int **)malloc(rows * sizeof(int *));
    if (mine == NULL) {
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        mine[i] = (int *)malloc(cols * sizeof(int));
        if (mine[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(mine[j]);
            }
            free(mine);
            return NULL;
        }
    }

    return mine;
}

void free_mine(int **mine, int rows) {
    if (mine == NULL) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        free(mine[i]);
    }
    free(mine);
}

int main() {
    int rows = 4;
    int cols = 4;
    
    int gold_data[4][4] = {
        {1, 3, 1, 5},
        {2, 2, 4, 1},
        {5, 0, 2, 3},
        {0, 6, 1, 2}
    };

    int **mine = allocate_mine(rows, cols);
    if (mine == NULL) {
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mine[i][j] = gold_data[i][j];
        }
    }

    int result = collect_gold(mine, rows, cols);
    printf("%d\n", result);

    free_mine(mine, rows);

    return 0;
}