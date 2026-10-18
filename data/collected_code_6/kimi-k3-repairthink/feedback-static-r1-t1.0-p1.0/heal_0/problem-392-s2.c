#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000000

long long dp[MAX_N + 1];

long long maxSum(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n <= MAX_N && dp[n] != -1) {
        return dp[n];
    }
    
    long long sum = maxSum(n / 2) + maxSum(n / 3) + maxSum(n / 4) + maxSum(n / 5);
    long long result = (sum > n) ? sum : n;
    
    if (n <= MAX_N) {
        dp[n] = result;
    }
    
    return result;
}

int main(void) {
    int n;
    
    for (int i = 0; i <= MAX_N; i++) {
        dp[i] = -1;
    }
    
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("%lld\n", maxSum(n));
    
    return EXIT_SUCCESS;
}