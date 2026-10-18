#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool is_prime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

long long nth_nsw_prime(int n) {
    if (n <= 0) return -1;
    
    int count = 0;
    long long s0 = 1, s1 = 1;
    
    while (1) {
        long long s2 = 2 * s1 + s0;
        if (is_prime(s2)) {
            count++;
            if (count == n) return s2;
        }
        s0 = s1;
        s1 = s2;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    printf("%lld\n", nth_nsw_prime(n));
    return 0;
}