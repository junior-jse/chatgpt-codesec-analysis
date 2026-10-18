#include <stdio.h>
#include <stdlib.h>

long long *memo;
size_t memo_size;
size_t memo_capacity;

long long max(long long a, long long b) {
    return a > b ? a : b;
}

long long f(long long n) {
    if (n < 4) {
        return n;
    }

    if (n < (long long)memo_size && memo[n] != -1) {
        return memo[n];
    }

    long long sum = f(n / 2) + f(n / 3) + f(n / 4) + f(n / 5);
    long long result = max(sum, n);

    if (n < (long long)memo_size) {
        memo[n] = result;
    }

    return result;
}

long long solve(long long n) {
    memo_capacity = n < 1000000 ? (size_t)n + 1 : 1000000;
    memo_size = memo_capacity;
    memo = (long long *)malloc(memo_size * sizeof(long long));
    if (memo == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < memo_size; i++) {
        memo[i] = -1;
    }

    long long result = f(n);
    free(memo);
    memo = NULL;
    memo_size = 0;
    memo_capacity = 0;

    return result;
}

int main(void) {
    long long n;

    while (scanf("%lld", &n) == 1) {
        printf("%lld\n", solve(n));
    }

    return 0;
}