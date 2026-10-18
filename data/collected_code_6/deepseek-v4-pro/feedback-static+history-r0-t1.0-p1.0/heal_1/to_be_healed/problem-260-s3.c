#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

static unsigned long long modular_pow(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static int is_prime_ull(unsigned long long n) {
    /* Possible weaknesses found:
     *  The scope of the variable 'a' can be reduced. [variableScope]
     */
    unsigned long long d, s, a;
    int i, j;
    
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0) return 0;
    if (n % 3 == 0) return 0;
    
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    
    /* Possible weaknesses found:
     *  Variable 'bases' can be declared as const array [constVariable]
     */
    unsigned long long bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    int num_bases = 7;
    
    for (i = 0; i < num_bases; i++) {
        a = bases[i] % n;
        if (a == 0) continue;
        
        unsigned long long x = modular_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        
        int composite = 1;
        for (j = 0; j < (int)s - 1; j++) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = 0;
                break;
            }
        }
        if (composite) return 0;
    }
    return 1;
}

static unsigned long long nsw_prime(unsigned long long n) {
    if (n == 0) {
        fprintf(stderr, "Index must be positive\n");
        exit(EXIT_FAILURE);
    }
    
    unsigned long long count = 0;
    unsigned long long k = 1;
    
    while (1) {
        unsigned long long s = (1ULL << k) - 1;
        unsigned long long val = (s * 2 + 1) * s / 2;
        
        if (is_prime_ull(val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        k++;
        
        /* Possible weaknesses found:
         *  Condition '(1ULL<<(k-1))==0' is always false [knownConditionTrueFalse]
         */
        if (k > 63 && (1ULL << (k - 1)) == 0) {
            fprintf(stderr, "Overflow detected\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char **argv) {
    unsigned long long n, result;
    char *endptr;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    errno = 0;
    n = strtoull(argv[1], &endptr, 10);
    
    if (errno == ERANGE && n == ULLONG_MAX) {
        fprintf(stderr, "Input out of range\n");
        return EXIT_FAILURE;
    }
    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    if (n == 0) {
        fprintf(stderr, "Index must be positive\n");
        return EXIT_FAILURE;
    }
    
    result = nsw_prime(n);
    printf("%llu\n", result);
    
    return EXIT_SUCCESS;
}