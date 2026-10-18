#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int longestRepeatingSubseq(const char *str, size_t len) {
    if (len <= 0) {
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
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
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

int main() {
    char str1[] = "aabebcdd";
    size_t len1 = sizeof(str1) - 1;
    int res1 = longestRepeatingSubseq(str1, len1);
    printf("%d\n", res1);
    
    char str2[] = "axxxy";
    size_t len2 = sizeof(str2) - 1;
    int res2 = longestRepeatingSubseq(str2, len2);
    printf("%d\n", res2);
    
    return 0;
}