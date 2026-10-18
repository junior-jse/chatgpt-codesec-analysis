#include <stdio.h>
#include <stdlib.h>

long long* memo;

long long max(long long a, long long b) {
    return (a > b) ? a : b;
}

long long f(long long n) {
    if (n == 0) return 0;
    
    if (n < 1000000 && memo[n] != -1) {
        return memo[n];
    }
    
    long long result = max(n, f(n/2) + f(n/3) + f(n/4) + f(n/5));
    
    if (n < 1000000) {
        memo[n] = result;
    }
    
    return result;
}

int main() {
    memo = (long long*)malloc(1000000 * sizeof(long long));
    
    for (int i = 0; i < 1000000; i++) {
        memo[i] = -1;
    }
    
    long long n;
    
    while (scanf("%lld", &n) != EOF) {
        printf("%lld\n", f(n));
    }
    
    free(memo);
    
    return 0;
}