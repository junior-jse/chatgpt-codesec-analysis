#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t strnlen_safe(const char *str, size_t max_len) {
    const char *end = memchr(str, '\0', max_len);
    if (end == NULL) {
        return max_len;
    }
    return end - str;
}

int shortest_supersequence(const char *str1, size_t max1, const char *str2, size_t max2) {
    size_t m = strnlen_safe(str1, max1);
    size_t n = strnlen_safe(str2, max2);
    size_t *dp = (size_t *)malloc((m + 1) * (n + 1) * sizeof(size_t));
    if (dp == NULL) {
        return -1;
    }
    for (size_t i = 0; i <= m; i++) {
        for (size_t j = 0; j <= n; j++) {
            if (i == 0) {
                dp[i * (n + 1) + j] = j;
            } else if (j == 0) {
                dp[i * (n + 1) + j] = i;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i * (n + 1) + j] = 1 + dp[(i - 1) * (n + 1) + (j - 1)];
            } else {
                size_t val1 = dp[(i - 1) * (n + 1) + j];
                size_t val2 = dp[i * (n + 1) + (j - 1)];
                dp[i * (n + 1) + j] = 1 + (val1 > val2 ? val2 : val1);
            }
        }
    }
    int result = (int)dp[m * (n + 1) + n];
    free(dp);
    return result;
}

int main() {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    int result = shortest_supersequence(str1, 1024, str2, 1024);
    printf("%d\n", result);
    return 0;
}