#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static uint64_t nth_nsw_prime(int n) {
    if (n < 1) return 0;
    
    uint64_t s0 = 1, s1 = 1;
    int count = 0;
    
    if (is_prime(s0)) count++;
    if (count == n) return s0;
    
    if (is_prime(s1)) count++;
    if (count == n) return s1;
    
    uint64_t s_curr;
    while (count < n) {
        s_curr = 2 * s1 + s0;
        if (is_prime(s_curr)) {
            count++;
            if (count == n) return s_curr;
        }
        s0 = s1;
        s1 = s_curr;
    }
    
    return s_curr;
}

int main(void) {
    int n;
    
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (n < 1) {
        fprintf(stderr, "n must be positive\n");
        return EXIT_FAILURE;
    }
    
    uint64_t result = nth_nsw_prime(n);
    printf("%lu\n", result);
    
    return EXIT_SUCCESS;
}