#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 1000000

long long table[TABLE_SIZE] = {0};

long long f(long long n) {
    if (n < 0) {
        return 0;
    }
    if (n == 0) {
        return 0;
    }
    if (n < TABLE_SIZE) {
        if (table[n] != 0) {
            return table[n];
        }
    }
    
    long long sum = f(n / 2) + f(n / 3) + f(n / 4) + f(n / 5);
    long long result = sum > n ? sum : n;
    
    if (n < TABLE_SIZE) {
        table[n] = result;
    }
    
    return result;
}

int main(void) {
    long long n;
    if (scanf("%lld", &n) == 1) {
        printf("%lld\n", f(n));
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}