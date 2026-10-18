#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

bool is_woodall(unsigned long long n) {
    if (n == 0) {
        return false;
    }
    unsigned long long i = 1;
    unsigned long long power = 2;
    unsigned long long result = power * i - 1;
    while (result < n) {
        i++;
        if (power > (ULLONG_MAX / 2)) {
            return false;
        }
        power *= 2;
        if (i > (ULLONG_MAX / power)) {
            return false;
        }
        result = power * i - 1;
    }
    return result == n;
}

int main(void) {
    unsigned long long num;
    if (scanf("%llu", &num) != 1) {
        return 1;
    }
    if (is_woodall(num)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    return 0;
}