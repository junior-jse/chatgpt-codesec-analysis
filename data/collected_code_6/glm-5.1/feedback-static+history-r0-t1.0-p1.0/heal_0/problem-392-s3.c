#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000000

long long cache[MAX_SIZE];

long long f(long long n) {
    if (n >= 0 && n < MAX_SIZE && cache[n] != -1) {
        return cache[n];
    }
    
    long long sum = f(n / 2) + f(n / 3) + f(n / 4) + f(n / 5);
    long long result = n > sum ? n : sum;
    
    if (n >= 0 && n < MAX_SIZE) {
        cache[n] = result;
    }
    
    return result;
}

int main(void) {
    long long n;
    
    for (long long i = 0; i < MAX_SIZE; i++) {
        cache[i] = -1;
    }
    cache[0] = 0;
    
    if (scanf("%lld", &n) == 1) {
        printf("%lld\n", f(n));
    }
    
    return 0;
}