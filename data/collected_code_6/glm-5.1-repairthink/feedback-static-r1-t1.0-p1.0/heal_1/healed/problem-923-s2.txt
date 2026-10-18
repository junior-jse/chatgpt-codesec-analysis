#include <stdio.h>
#include <stdlib.h>

int shortest_supersequence(const char *str1, size_t m, const char *str2, size_t n) {
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i <= m; i++) {
        dp[i] = (int *)malloc((n + 1) * sizeof(int));
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
                dp[i][j] = (int)j;
            } else if (j == 0) {
                dp[i][j] = (int)i;
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
    char str1[] = "AGGTAB";
    char str2[] = "GXTXAYB";
    
    size_t m = sizeof(str1) - 1;
    size_t n = sizeof(str2) - 1;
    
    int length = shortest_supersequence(str1, m, str2, n);
    
    if (length != -1) {
        printf("%d\n", length);
    }
    
    return 0;
}