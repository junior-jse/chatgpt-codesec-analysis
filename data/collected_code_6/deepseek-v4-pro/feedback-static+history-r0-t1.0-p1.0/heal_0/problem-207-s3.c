#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int longest_repeating_subsequence(const char *str) {
    int n = (int)strlen(str);
    if (n == 0) return 0;

    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    if (!dp) return -1;

    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)calloc(n + 1, sizeof(int));
        if (!dp[i]) {
            for (int k = 0; k < i; k++) {
                free(dp[k]);
            }
            free(dp);
            return -1;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
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

int main(void) {
    const char *test1 = "ATACTCGGA";
    const char *test2 = "AABEBCDD";
    const char *test3 = "abcabc";
    const char *test4 = "";
    const char *test5 = "aaaa";

    printf("%d\n", longest_repeating_subsequence(test1));
    printf("%d\n", longest_repeating_subsequence(test2));
    printf("%d\n", longest_repeating_subsequence(test3));
    printf("%d\n", longest_repeating_subsequence(test4));
    printf("%d\n", longest_repeating_subsequence(test5));

    return 0;
}