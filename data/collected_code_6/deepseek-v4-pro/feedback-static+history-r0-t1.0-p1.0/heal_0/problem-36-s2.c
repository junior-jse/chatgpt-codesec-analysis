#include <stdio.h>
#include <stdlib.h>

int nth_digit(int numerator, int denominator, int n) {
    if (denominator == 0) {
        fprintf(stderr, "Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }
    if (numerator < 0 || denominator < 0) {
        fprintf(stderr, "Error: Negative values are not allowed\n");
        exit(EXIT_FAILURE);
    }
    if (numerator >= denominator) {
        numerator %= denominator;
    }
    if (numerator == 0) {
        return 0;
    }

    long long remainder = numerator;
    for (int i = 0; i <= n; i++) {
        if (remainder == 0) {
            return 0;
        }
        remainder *= 10;
        int digit = remainder / denominator;
        remainder %= denominator;
        if (i == n) {
            return digit;
        }
    }
    return 0;
}

int main(void) {
    int num, den, n;
    printf("Enter numerator: ");
    if (scanf("%d", &num) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    printf("Enter denominator: ");
    if (scanf("%d", &den) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    printf("Enter digit position (0-indexed): ");
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    int digit = nth_digit(num, den, n);
    printf("Digit at position %d: %d\n", n, digit);

    return EXIT_SUCCESS;
}