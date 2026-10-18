#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    unsigned long long result = nsw_prime(n);
    printf("%llu\n", result);
    return EXIT_SUCCESS;
}