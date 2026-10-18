#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int shortest_common_supersequence_length(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t m = strlen(str1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t n = strlen(str2);
    
    if (m > (size_t)(-1) / sizeof(int *) || n > (size_t)(-1) / sizeof(int) - 1) {
        return -1;
    }
    
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
        dp[i][0] = (int)i;
    }
    for (size_t j = 0; j <= n; j++) {
        dp[0][j] = (int)j;
    }
    
    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
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
    
    for (size_t i = 0; i <= m; i++) {
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