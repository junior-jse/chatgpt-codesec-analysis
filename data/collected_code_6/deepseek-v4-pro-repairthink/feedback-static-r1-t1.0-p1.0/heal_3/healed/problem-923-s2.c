#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

size_t shortest_supersequence_length(const char *str1, size_t len1,
                                     const char *str2, size_t len2) {
    if (str1 == NULL || str2 == NULL) {
        return (size_t)-1;
    }
    size_t m = len1;
    size_t n = len2;

    size_t *dp = malloc((m + 1) * (n + 1) * sizeof(size_t));
    if (dp == NULL) {
        return (size_t)-1;
    }

    for (size_t i = 0; i <= m; i++) {
        dp[i * (n + 1) + 0] = i;
    }
    for (size_t j = 0; j <= n; j++) {
        dp[0 * (n + 1) + j] = j;
    }

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)] + 1;
            } else {
                size_t a = dp[(i - 1) * (n + 1) + j];
                size_t b = dp[i * (n + 1) + (j - 1)];
                size_t min = a < b ? a : b;
                dp[i * (n + 1) + j] = min + 1;
            }
        }
    }

    size_t result = dp[m * (n + 1) + n];

    free(dp);
    return result;
}

int main(void) {
    char str1[] = "AGGTAB";
    char str2[] = "GXTXAYB";
    size_t len1 = sizeof(str1) - 1;
    size_t len2 = sizeof(str2) - 1;

    size_t length = shortest_supersequence_length(str1, len1, str2, len2);

    if (length != (size_t)-1) {
        printf("Length of shortest supersequence: %zu\n", length);
    } else {
        printf("Memory allocation failed\n");
    }

    return 0;
}