#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int maxSegments(int n, int a, int b, int c) {
    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        return -1;
    }
    
    if (n == 0) {
        return 0;
    }
    
    int *dp = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (dp == NULL) {
        return -1;
    }
    
    dp[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        dp[i] = -1;
        
        if (i >= a && dp[i - a] != -1) {
            int candidate = dp[i - a] + 1;
            if (candidate > dp[i]) {
                dp[i] = candidate;
            }
        }
        
        if (i >= b && dp[i - b] != -1) {
            int candidate = dp[i - b] + 1;
            if (candidate > dp[i]) {
                dp[i] = candidate;
            }
        }
        
        if (i >= c && dp[i - c] != -1) {
            int candidate = dp[i - c] + 1;
            if (candidate > dp[i]) {
                dp[i] = candidate;
            }
        }
    }
    
    int result = dp[n];
    free(dp);
    dp = NULL;
    
    return result;
}

int main(void) {
    int n, a, b, c;
    
    if (scanf("%d %d %d %d", &n, &a, &b, &c) != 4) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        fprintf(stderr, "Invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    int result = maxSegments(n, a, b, c);
    
    if (result == -1) {
        printf("0\n");
    } else {
        printf("%d\n", result);
    }
    
    return EXIT_SUCCESS;
}