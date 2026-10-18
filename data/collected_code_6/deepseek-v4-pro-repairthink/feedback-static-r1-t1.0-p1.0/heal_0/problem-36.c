#include <stdio.h>
#include <stdlib.h>

int nth_digit_fraction(int numerator, int denominator, int n) {
    if (denominator == 0) {
        return -1;
    }
    if (numerator < 0 || denominator < 0 || n < 1) {
        return -1;
    }
    if (numerator >= denominator) {
        numerator = numerator % denominator;
    }
    if (numerator == 0) {
        return 0;
    }

    int digit = 0;
    for (int i = 0; i < n; i++) {
        numerator *= 10;
        digit = numerator / denominator;
        numerator = numerator % denominator;
        if (numerator == 0 && i < n - 1) {
            return 0;
        }
    }
    return digit;
}

int main(void) {
    int num, den, n;
    
    printf("Enter numerator: ");
    if (scanf("%d", &num) != 1) {
        return 1;
    }
    printf("Enter denominator: ");
    if (scanf("%d", &den) != 1) {
        return 1;
    }
    printf("Enter position: ");
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    int result = nth_digit_fraction(num, den, n);
    if (result == -1) {
        printf("Invalid input\n");
    } else {
        printf("Digit at position %d: %d\n", n, result);
    }

    return 0;
}