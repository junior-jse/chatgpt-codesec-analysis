#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int max_segments(int n, int a, int b, int c) {
    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        return -1;
    }
    
    if (n == 0) {
        return 0;
    }
    
    int *dp = (int *)calloc((size_t)n + 1, sizeof(int));
    if (dp == NULL) {
        return -1;
    }
    
    dp[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        int max_val = INT_MIN;
        
        if (i >= a && dp[i - a] != INT_MIN) {
            int candidate = dp[i - a] + 1;
            if (candidate > max_val) {
                max_val = candidate;
            }
        }
        
        if (i >= b && dp[i - b] != INT_MIN) {
            int candidate = dp[i - b] + 1;
            if (candidate > max_val) {
                max_val = candidate;
            }
        }
        
        if (i >= c && dp[i - c] != INT_MIN) {
            int candidate = dp[i - c] + 1;
            if (candidate > max_val) {
                max_val = candidate;
            }
        }
        
        dp[i] = max_val;
    }
    
    int result = dp[n];
    free(dp);
    dp = NULL;
    
    return (result == INT_MIN) ? -1 : result;
}

int main(void) {
    int n, a, b, c;
    
    printf("Enter n, a, b, c: ");
    if (scanf("%d %d %d %d", &n, &a, &b, &c) != 4) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int result = max_segments(n, a, b, c);
    
    if (result >= 0) {
        printf("Maximum segments: %d\n", result);
    } else {
        printf("Cannot form segments\n");
    }
    
    return EXIT_SUCCESS;
}