#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int is_woodall(long long n) {
    if (n <= 0) {
        return 0;
    }
    
    long long k = 1;
    while (1) {
        if (k > (LLONG_MAX - 1) / k) {
            return 0;
        }
        
        long long woodall = k * (1LL << k) - 1;
        
        if (woodall == n) {
            return 1;
        }
        
        if (woodall > n) {
            return 0;
        }
        
        k++;
    }
}

int main(void) {
    long long num;
    
    if (scanf("%lld", &num) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (is_woodall(num)) {
        printf("%lld is a Woodall number\n", num);
    } else {
        printf("%lld is not a Woodall number\n", num);
    }
    
    return EXIT_SUCCESS;
}