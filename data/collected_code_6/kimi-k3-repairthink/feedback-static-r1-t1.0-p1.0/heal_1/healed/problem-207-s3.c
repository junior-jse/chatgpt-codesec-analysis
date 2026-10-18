#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#define MAX_INPUT_LENGTH 4096

static size_t boundedStringLength(const char *str, size_t maxLen) {
    size_t len = 0;
    while (len < maxLen && str[len] != '\0') {
        len++;
    }
    return len;
}

int longestRepeatingSubsequence(const char *str) {
    size_t n, i, j;
    int **dp;
    int result;

    if (str == NULL) {
        return 0;
    }

    n = boundedStringLength(str, MAX_INPUT_LENGTH);
    if (n == 0) {
        return 0;
    }

    dp = (int **)calloc(n + 1, sizeof(int *));
    if (dp == NULL) {
        return 0;
    }

    for (i = 0; i <= n; i++) {
        dp[i] = (int *)calloc(n + 1, sizeof(int));
        if (dp[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
        }
    }

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    result = dp[n][n];

    for (i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

int main(void) {
    char str1[] = "aabb";
    char str2[] = "aabebcdd";
    char str3[] = "";

    printf("Longest repeating subsequence in \"%s\": %d\n", str1, longestRepeatingSubsequence(str1));
    printf("Longest repeating subsequence in \"%s\": %d\n", str2, longestRepeatingSubsequence(str2));
    printf("Longest repeating subsequence in \"%s\": %d\n", str3, longestRepeatingSubsequence(str3));

    return 0;
}