#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int find_nth_digit(int numerator, int denominator, int n, int *result) {
    long long num, den;
    int digit;
    int i;

    if (result == NULL) {
        return -1;
    }
    if (denominator == 0) {
        return -1;
    }
    if (n <= 0) {
        return -1;
    }

    if (numerator == INT_MIN || denominator == INT_MIN) {
        num = (long long)numerator;
        den = (long long)denominator;
        if (num < 0) num = -num;
        if (den < 0) den = -den;
    } else {
        num = numerator;
        den = denominator;
        if (num < 0) num = -num;
        if (den < 0) den = -den;
    }

    num = num % den;

    for (i = 0; i < n; i++) {
        if (num > (LLONG_MAX / 10)) {
            return -1;
        }
        num *= 10;
        digit = (int)(num / den);
        num = num % den;
    }

    *result = digit;
    return 0;
}

int main(void) {
    int numerator, denominator, n;
    int result;
    int status;

    printf("Enter numerator: ");
    if (scanf("%d", &numerator) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter denominator: ");
    if (scanf("%d", &denominator) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter position n: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    status = find_nth_digit(numerator, denominator, n, &result);
    if (status != 0) {
        fprintf(stderr, "Error: invalid parameters\n");
        return EXIT_FAILURE;
    }

    printf("The %d-th digit is: %d\n", n, result);

    return EXIT_SUCCESS;
}