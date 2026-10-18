#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

long long find_nth_digit(long long numerator, long long denominator, int n) {
    if (denominator <= 0 || n <= 0) {
        return -1;
    }
    if (numerator < 0) {
        numerator = -numerator;
    }
    long long remainder = numerator % denominator;
    for (int i = 0; i < n; ++i) {
        remainder *= 10;
        int digit = (int)(remainder / denominator);
        remainder %= denominator;
        if (i == n - 1) {
            return digit;
        }
    }
    return -1;
}

long long safe_strtoll(const char *str) {
    char *endptr;
    errno = 0;
    long long val = strtoll(str, &endptr, 10);
    if (errno == ERANGE || endptr == str || *endptr != '\0') {
        return LLONG_MIN;
    }
    return val;
}

int main(int argc, char * const argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <numerator> <denominator> <n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    long long numerator = safe_strtoll(argv[1]);
    long long denominator = safe_strtoll(argv[2]);
    long long n = safe_strtoll(argv[3]);
    if (numerator == LLONG_MIN || denominator == LLONG_MIN || n == LLONG_MIN || n > INT_MAX) {
        return EXIT_FAILURE;
    }
    long long result = find_nth_digit(numerator, denominator, (int)n);
    if (result == -1) {
        return EXIT_FAILURE;
    }
    printf("%lld\n", result);
    return EXIT_SUCCESS;
}