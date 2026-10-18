#include <stdio.h>
#include <limits.h>

int maxSegments(int n, int a, int b, int c) {
    int dp[n + 1];
    
    for (int i = 0; i <= n; i++) {
        dp[i] = -1;
    }
    
    dp[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        int max_val = -1;
        
        if (i >= a && dp[i - a] != -1) {
            if (dp[i - a] + 1 > max_val) {
                max_val = dp[i - a] + 1;
            }
        }
        
        if (i >= b && dp[i - b] != -1) {
            if (dp[i - b] + 1 > max_val) {
                max_val = dp[i - b] + 1;
            }
        }
        
        if (i >= c && dp[i - c] != -1) {
            if (dp[i - c] + 1 > max_val) {
                max_val = dp[i - c] + 1;
            }
        }
        
        dp[i] = max_val;
    }
    
    if (dp[n] == -1) {
        return 0;
    }
    
    return dp[n];
}

int main(void) {
    int n, a, b, c;
    
    printf("Enter n: ");
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    
    printf("Enter a, b, c: ");
    if (scanf("%d %d %d", &a, &b, &c) != 3) {
        return 1;
    }
    
    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        return 1;
    }
    
    int result = maxSegments(n, a, b, c);
    printf("%d\n", result);
    
    return 0;
}