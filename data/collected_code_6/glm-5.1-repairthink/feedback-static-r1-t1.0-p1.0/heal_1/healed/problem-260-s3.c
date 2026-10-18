#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

unsigned long long nsw_prime(unsigned int n) {
    unsigned long long a = 1;
    unsigned long long b = 1;
    for (unsigned int i = 1; i <= n; i++) {
        unsigned long long c = (2ULL * i * b) + a;
        a = b;
        b = c;
    }
    return b;
}

int main(int argc, const char * const argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    
    errno = 0;
    char *endptr;
    unsigned long long n_ll = strtoull(argv[1], &endptr, 10);
    
    if (errno != 0 || endptr == argv[1] || *endptr != '\0' || n_ll > UINT_MAX) {
        return EXIT_FAILURE;
    }
    
    unsigned int n = (unsigned int)n_ll;
    unsigned long long result = nsw_prime(n);
    printf("%llu\n", result);
    
    return EXIT_SUCCESS;
}