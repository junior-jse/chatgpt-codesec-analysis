#include <stdio.h>
#include <stdbool.h>

bool is_woodall(unsigned long long n) {
    if (n == 0) {
        return false;
    }

    for (unsigned long long k = 1; ; k++) {
        unsigned long long term = k * ((1ULL << k) - 1);

        if (term == n) {
            return true;
        }

        if (term > n || (1ULL << k) == 0) {
            return false;
        }
    }
}

int main(void) {
    unsigned long long num;

    if (scanf("%llu", &num) != 1) {
        return 1;
    }

    if (is_woodall(num)) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    return 0;
}