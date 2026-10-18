#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool is_prime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

long long nsw_prime(int n) {
    if (n <= 0) return -1;
    long long a = 1;
    long long b = 1;
    int count = 0;
    long long candidate = 1;
    while (count < n) {
        long long next = 2 * b + a;
        a = b;
        b = next;
        if (is_prime(next)) {
            count++;
            candidate = next;
        }
    }
    return candidate;
}

int main(void) {
    int n;
    printf("Enter n: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    printf("%lld\n", nsw_prime(n));
    return EXIT_SUCCESS;
}