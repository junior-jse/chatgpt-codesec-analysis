#include <stdio.h>
#include <stdlib.h>

int nth_digit(int numerator, int denominator, int n) {
    if (denominator == 0 || n < 1) {
        return -1;
    }

    long long num = numerator;
    long long den = denominator;
    int digit = -1;

    for (int i = 0; i < n; i++) {
        if (num == 0) {
            return 0;
        }
        num *= 10;
        digit = (int)(num / den);
        num %= den;
    }

    return digit;
}

int main(void) {
    int numerator = 1;
    int denominator = 7;
    int n = 5;

    int result = nth_digit(numerator, denominator, n);

    if (result >= 0) {
        printf("%d\n", result);
    } else {
        printf("Invalid input\n");
    }

    return 0;
}