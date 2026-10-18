#include <stdio.h>
#include <stdlib.h>

unsigned long long find_nsw_prime(int n) {
    if (n <= 0) {
        exit(EXIT_FAILURE);
    }
    if (n == 1) {
        return 1;
    }
    unsigned long long a = 1;
    unsigned long long b = 1;
    unsigned long long c;
    for (int i = 2; i <= n; i++) {
        c = 2 * b + a;
        a = b;
        b = c;
    }
    return b;
}

int main(int argc, char *argv[]) {
    int n;
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    n = atoi(argv[1]);
    if (n <= 0) {
        return EXIT_FAILURE;
    }
    unsigned long long result = find_nsw_prime(n);
    printf("%llu\n", result);
    return EXIT_SUCCESS;
}