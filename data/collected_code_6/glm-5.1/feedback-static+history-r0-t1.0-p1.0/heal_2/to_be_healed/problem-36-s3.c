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
    numerator %= denominator;
    for (int i = 0; i < n; i++) {
        numerator *= 10;
        if (numerator >= denominator) {
            if (i == n - 1) {
                return numerator / denominator;
            }
            numerator %= denominator;
        } else {
            if (i == n - 1) {
                return 0;
            }
        }
    }
    return -1;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <numerator> <denominator> <n>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *endptr;
    long val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        return EXIT_FAILURE;
    }
    int numerator = (int)val;
    val = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || val == 0) {
        return EXIT_FAILURE;
    }
    int denominator = (int)val;
    val = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || val <= 0) {
        return EXIT_FAILURE;
    }
    int n = (int)val;
    int result = find_nth_digit(numerator, denominator, n);
    if (result >= 0 && result <= 9) {
        printf("%d\n", result);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}