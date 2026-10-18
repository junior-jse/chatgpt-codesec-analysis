#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longestRepeatingSubsequence(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0) {
        return 0;
    }
    
    int **dp = malloc((len + 1) * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i <= len; i++) {
        dp[i] = calloc(len + 1, sizeof(int));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return 0;
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
    const char *test1 = "aabb";
    const char *test2 = "aab";
    const char *test3 = "aabebcdd";
    const char *test4 = "";
    
    printf("Longest repeating subsequence in \"%s\": %d\n", test1, longestRepeatingSubsequence(test1));
    printf("Longest repeating subsequence in \"%s\": %d\n", test2, longestRepeatingSubsequence(test2));
    printf("Longest repeating subsequence in \"%s\": %d\n", test3, longestRepeatingSubsequence(test3));
    printf("Longest repeating subsequence in \"%s\": %d\n", test4, longestRepeatingSubsequence(test4));
    
    return 0;
}