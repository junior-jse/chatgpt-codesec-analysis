#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int shortest_common_supersequence_length(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t n1 = strlen(str1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t n2 = strlen(str2);
    if (n1 > INT_MAX || n2 > INT_MAX) {
        return -1;
    }
    int len1 = (int)n1;
    int len2 = (int)n2;
    
    size_t rows = (size_t)len1 + 1;
    size_t cols = (size_t)len2 + 1;
    
    int **dp = malloc(rows * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }
    
    int alloc_failed = 0;
    /* Possible weaknesses found:
     *  Shadowed declaration
     */
    size_t i;
    for (i = 0; i < rows; i++) {
        dp[i] = malloc(cols * sizeof(int));
        if (dp[i] == NULL) {
            alloc_failed = 1;
            break;
        }
    }
    
    if (alloc_failed) {
        for (size_t j = 0; j < i; j++) {
            free(dp[j]);
        }
        free(dp);
        return -1;
    }
    
    /* Possible weaknesses found:
     *  Shadow variable
     *  Local variable 'i' shadows outer variable [shadowVariable]
     */
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }
    
    int lcs_length = dp[len1][len2];
    
    /* Possible weaknesses found:
     *  Shadow variable
     *  Local variable 'i' shadows outer variable [shadowVariable]
     */
    for (size_t i = 0; i < rows; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return len1 + len2 - lcs_length;
}

int main(void) {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    
    int result = shortest_common_supersequence_length(str1, str2);
    
    if (result != -1) {
        printf("Length of shortest common supersequence: %d\n", result);
    } else {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}