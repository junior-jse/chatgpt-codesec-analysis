#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

size_t shortest_supersequence_length(const char *str1, size_t len1,
                                     const char *str2, size_t len2) {
    if (str1 == NULL || str2 == NULL) {
        return (size_t)-1;
    }
    size_t m = len1;
    size_t n = len2;

    size_t **dp = malloc((m + 1) * sizeof(size_t *));
    if (dp == NULL) {
        return (size_t)-1;
    }

    for (size_t i = 0; i <= m; i++) {
        dp[i] = malloc((n + 1) * sizeof(size_t));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return (size_t)-1;
        }
    }

    for (size_t i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (size_t j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                size_t min = dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
                dp[i][j] = min + 1;
            }
        }
    }

    size_t result = dp[m][n];

    for (size_t i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

int main(void) {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    size_t length = shortest_supersequence_length(str1, len1, str2, len2);

    if (length != (size_t)-1) {
        printf("Length of shortest supersequence: %zu\n", length);
    } else {
        printf("Memory allocation failed\n");
    }

    return 0;
}