#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int longestRepeatingSubseq(const char *str, int n) {
    if (n == 0) {
        return 0;
    }
    int **dp = malloc((n + 1) * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }
    for (int i = 0; i <= n; i++) {
        dp[i] = malloc((n + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
        }
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    int result = dp[n][n];
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    return result;
}

int main() {
    char str1[] = "aabebcdd";
    int res1 = longestRepeatingSubseq(str1, sizeof(str1) - 1);
    printf("%d\n", res1);
    char str2[] = "axxxy";
    int res2 = longestRepeatingSubseq(str2, sizeof(str2) - 1);
    printf("%d\n", res2);
    char str3[] = "abc";
    int res3 = longestRepeatingSubseq(str3, sizeof(str3) - 1);
    printf("%d\n", res3);
    return 0;
}