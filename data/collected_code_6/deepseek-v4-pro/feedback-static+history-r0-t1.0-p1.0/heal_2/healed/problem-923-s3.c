#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

int shortest_common_supersequence_length(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    
    size_t len1 = strnlen(str1, SIZE_MAX / 2);
    size_t len2 = strnlen(str2, SIZE_MAX / 2);
    
    if (len1 > SIZE_MAX / 2 - 1 || len2 > SIZE_MAX / 2 - 1) {
        return -1;
    }
    
    if (len1 > SIZE_MAX / sizeof(int *) - 1 || len2 > SIZE_MAX / sizeof(int) - 1) {
        return -1;
    }
    
    int **dp = (int **)malloc((len1 + 1) * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }
    
    size_t i;
    for (i = 0; i <= len1; i++) {
        dp[i] = (int *)malloc((len2 + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return -1;
        }
    }
    
    for (i = 0; i <= len1; i++) {
        for (size_t j = 0; j <= len2; j++) {
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
    
    for (i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);
    
    if (len1 > SIZE_MAX - len2) {
        return -1;
    }
    
    return (int)(len1 + len2 - (size_t)lcs_length);
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