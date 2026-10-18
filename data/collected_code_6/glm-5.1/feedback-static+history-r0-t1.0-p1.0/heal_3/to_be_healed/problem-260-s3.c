#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned long long nsw_prime(int n) {
    if (n < 0) {
        exit(EXIT_FAILURE);
    }
    if (n == 0) {
        return 1;
    }
    unsigned long long a = 1;
    unsigned long long b = 1;
    for (int i = 1; i <= n; i++) {
        unsigned long long c = (2 * i * b) + a;
        a = b;
        b = c;
    }
    return b;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
 */
int main(int argc, char * const argv[const]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    long val = strtol(argv[1], NULL, 10);
    if (val < 0 || val > INT_MAX) {
        return EXIT_FAILURE;
    }
    int n = (int)val;
    unsigned long long result = nsw_prime(n);
    printf("%llu\n", result);
    return EXIT_SUCCESS;
}