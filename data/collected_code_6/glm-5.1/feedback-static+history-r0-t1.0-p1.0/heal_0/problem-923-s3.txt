#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shortest_common_supersequence_len(const char *str1, const char *str2) {
    size_t m = strlen(str1);
    size_t n = strlen(str2);
    
    size_t *dp = (size_t *)malloc((n + 1) * sizeof(size_t));
    if (dp == NULL) {
        exit(EXIT_FAILURE);
    }
    
    for (size_t j = 0; j <= n; ++j) {
        dp[j] = j;
    }
    
    for (size_t i = 1; i <= m; ++i) {
        size_t prev = dp[0];
        dp[0] = i;
        for (size_t j = 1; j <= n; ++j) {
            size_t temp = dp[j];
            if (str1[i - 1] == str2[j - 1]) {
                dp[j] = prev + 1;
            } else {
                dp[j] = (dp[j] < dp[j - 1]) ? dp[j] + 1 : dp[j - 1] + 1;
            }
            prev = temp;
        }
    }
    
    int result = (int)dp[n];
    free(dp);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <str1> <str2>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    if (argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "Invalid input strings.\n");
        return EXIT_FAILURE;
    }
    
    int length = shortest_common_supersequence_len(argv[1], argv[2]);
    printf("%d\n", length);
    
    return EXIT_SUCCESS;
}