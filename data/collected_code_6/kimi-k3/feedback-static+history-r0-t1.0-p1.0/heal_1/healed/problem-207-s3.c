#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int longestRepeatingSubsequence(const char *str, size_t len) {
    if (str == NULL) {
        return 0;
    }
    
    if (len == 0) {
        return 0;
    }
    
    int **dp = (int **)malloc((len + 1) * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i <= len; i++) {
        dp[i] = (int *)malloc((len + 1) * sizeof(int));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
        }
    }
    
    for (size_t i = 0; i <= len; i++) {
        for (size_t j = 0; j <= len; j++) {
            dp[i][j] = 0;
        }
    }
    
    for (size_t i = 1; i <= len; i++) {
        for (size_t j = 1; j <= len; j++) {
            if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }
    
    int result = dp[len][len];
    
    for (size_t i = 0; i <= len; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

int main(void) {
    char str1[] = "aabb";
    char str2[] = "aabebcdd";
    char str3[] = "";
    
    printf("Longest repeating subsequence in \"%s\": %d\n", str1, longestRepeatingSubsequence(str1, sizeof(str1) - 1));
    printf("Longest repeating subsequence in \"%s\": %d\n", str2, longestRepeatingSubsequence(str2, sizeof(str2) - 1));
    printf("Longest repeating subsequence in \"%s\": %d\n", str3, longestRepeatingSubsequence(str3, sizeof(str3) - 1));
    
    return 0;
}