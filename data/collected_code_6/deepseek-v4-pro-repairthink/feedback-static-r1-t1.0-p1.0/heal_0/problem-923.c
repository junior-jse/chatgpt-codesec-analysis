#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int shortest_common_supersequence_length(const char *str1, const char *str2) {
    int m = strlen(str1);
    int n = strlen(str2);
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }
    for (int i = 0; i <= m; i++) {
        dp[i] = (int *)malloc((n + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return -1;
        }
    }
    
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                int left = dp[i - 1][j] + 1;
                int up = dp[i][j - 1] + 1;
                dp[i][j] = (left < up) ? left : up;
            }
        }
    }
    
    int result = dp[m][n];
    
    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

int main(void) {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    int length = shortest_common_supersequence_length(str1, str2);
    if (length >= 0) {
        printf("%d\n", length);
    }
    return 0;
}