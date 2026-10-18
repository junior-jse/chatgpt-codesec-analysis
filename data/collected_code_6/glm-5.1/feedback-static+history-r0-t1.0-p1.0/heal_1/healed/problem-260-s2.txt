#include <stdio.h>
#include <stdlib.h>

unsigned long long nsw_prime(int n) {
    if (n <= 0) {
        return 0;
    }
    
    unsigned long long prev = 1;
    unsigned long long curr = 1;
    
    for (int i = 2; i <= n; i++) {
        unsigned long long next = 2 * curr * curr + prev;
        if (next < curr) {
            return 0;
        }
        prev = curr;
        curr = next;
    }
    
    return curr;
}

int main(int argc, char * const argv[]) {
    int n = 5;
    
    if (argc == 2) {
        long val = strtol(argv[1], NULL, 10);
        if (val <= 0 || val > 20) {
            return 1;
        }
        n = (int)val;
    }
    
    unsigned long long result = nsw_prime(n);
    
    if (result == 0 && n > 0) {
        return 1;
    }
    
    printf("%llu\n", result);
    return 0;
}