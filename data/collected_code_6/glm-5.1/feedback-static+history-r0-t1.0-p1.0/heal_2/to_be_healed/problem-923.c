#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shortest_supersequence(const char *str1, const char *str2) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t m = strlen(str1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t n = strlen(str2);
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
    int result = shortest_supersequence(str1, str2);
    printf("%d\n", result);
    return 0;
}