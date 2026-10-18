#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

unsigned long long find_nsw_prime(int n) {
    if (n == 1) {
        return 1;
    }
    unsigned long long a = 1;
    unsigned long long b = 1;
    for (int i = 2; i <= n; i++) {
        unsigned long long c = 2 * b + a;
        a = b;
        b = c;
    }
    return b;
}

int main(int argc, char * const * const argv) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    char *endptr;
    errno = 0;
    long val = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0' || errno == ERANGE || val <= 0 || val > INT_MAX) {
        return EXIT_FAILURE;
    }
    int n = (int)val;
    unsigned long long result = find_nsw_prime(n);
    printf("%llu\n", result);
    return EXIT_SUCCESS;
}