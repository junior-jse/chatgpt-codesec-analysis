#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shortest_supersequence(const char *str1, size_t max_len1, const char *str2, size_t max_len2) {
    size_t len1 = strnlen(str1, max_len1);
    size_t len2 = strnlen(str2, max_len2);

    if (len1 == max_len1 || len2 == max_len2) {
        return -1;
    }

    size_t m = len1;
    size_t n = len2;

    int *dp_block = (int *)malloc((m + 1) * (n + 1) * sizeof(int));
    if (dp_block == NULL) {
        return -1;
    }

    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    if (dp == NULL) {
        free(dp_block);
        return -1;
    }

    for (size_t i = 0; i <= m; i++) {
        dp[i] = dp_block + i * (n + 1);
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

    free(dp);
    free(dp_block);

    return result;
}

int main() {
    const char str1[] = "AGGTAB";
    const char str2[] = "GXTXAYB";

    int length = shortest_supersequence(str1, sizeof(str1), str2, sizeof(str2));

    if (length != -1) {
        printf("%d\n", length);
    }

    return 0;
}