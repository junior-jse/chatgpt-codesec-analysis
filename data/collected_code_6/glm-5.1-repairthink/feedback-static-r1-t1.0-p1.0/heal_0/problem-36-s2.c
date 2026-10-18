#include <stdio.h>
#include <stdlib.h>

int find_nth_digit(int numerator, int denominator, int n) {
    if (denominator == 0 || n <= 0) {
        return -1;
    }

    if (numerator < 0) {
        numerator = -numerator;
    }
    if (denominator < 0) {
        denominator = -denominator;
    }

    numerator = numerator % denominator;

    for (int i = 0; i < n; i++) {
        numerator *= 10;
        if (numerator == 0 && i > 0) {
            return 0;
        }
    }

    int digit = (numerator / denominator) % 10;

    return digit;
}

int main(void) {
    int num, den, n;

    if (scanf("%d %d %d", &num, &den, &n) != 3) {
        return EXIT_FAILURE;
    }

    int result = find_nth_digit(num, den, n);

    printf("%d\n", result);

    return EXIT_SUCCESS;
}