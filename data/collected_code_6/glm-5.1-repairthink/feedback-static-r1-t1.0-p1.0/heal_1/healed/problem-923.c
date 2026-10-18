#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shortest_supersequence(const char *str1, size_t m, const char *str2, size_t n) {
    int **dp = malloc((m + 1) * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }
    for (size_t i = 0; i <= m; i++) {
        dp[i] = malloc((n + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return -1;
        }
    }
    for (size_t i = 0; i <= m; i++) {
        for (size_t j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + (dp[i - 1][j] > dp[i][j - 1] ? dp[i][j - 1] : dp[i - 1][j]);
            }
        }
    }
    int result = dp[m][n];
    for (size_t i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);
    return result;
}

int main() {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    size_t m = strlen(str1);
    size_t n = strlen(str2);
    int result = shortest_supersequence(str1, m, str2, n);
    printf("%d\n", result);
    return 0;
}