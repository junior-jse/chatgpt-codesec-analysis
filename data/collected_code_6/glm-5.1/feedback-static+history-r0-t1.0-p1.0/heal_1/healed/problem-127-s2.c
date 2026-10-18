#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int multiply(int a, int b) {
    long long result = 0;
    int negative = 0;

    if (a == 0 || b == 0) {
        return 0;
    }

    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
        negative = 1;
    }

    long long abs_a = a < 0 ? -(long long)a : (long long)a;
    long long abs_b = b < 0 ? -(long long)b : (long long)b;

    while (abs_b > 0) {
        if (abs_b & 1) {
            result += abs_a;
        }
        abs_a <<= 1;
        abs_b >>= 1;
    }

    if (negative) {
        result = -result;
    }

    if (result > INT_MAX || result < INT_MIN) {
        abort();
    }

    return (int)result;
}

int main(void) {
    int a = 0;
    int b = 0;

    if (scanf("%d", &a) != 1) {
        abort();
    }
    if (scanf("%d", &b) != 1) {
        abort();
    }

    int product = multiply(a, b);
    printf("%d\n", product);

    return 0;
}