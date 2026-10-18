#include <stdio.h>
#include <stdlib.h>

int find_nth_digit(int numerator, int denominator, int n) {
    if (denominator <= 0 || n <= 0) {
        return -1;
    }
    long long num = (long long)numerator;
    long long den = (long long)denominator;
    if (num < 0) {
        num = -num;
    }
    if (den < 0) {
        den = -den;
    }
    long long remainder = num % den;
    for (int i = 0; i < n; ++i) {
        remainder *= 10;
        if (den == 0) {
            return -1;
        }
        int digit = (int)(remainder / den);
        remainder %= den;
        if (i == n - 1) {
            return digit;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <numerator> <denominator> <n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int numerator = atoi(argv[1]);
    int denominator = atoi(argv[2]);
    int n = atoi(argv[3]);
    int result = find_nth_digit(numerator, denominator, n);
    if (result == -1) {
        return EXIT_FAILURE;
    }
    printf("%d\n", result);
    return EXIT_SUCCESS;
}