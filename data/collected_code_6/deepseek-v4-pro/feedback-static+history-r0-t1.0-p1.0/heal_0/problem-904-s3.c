#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool is_even(int number) {
    return (number % 2 == 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    long value = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || endptr == argv[1]) {
        fprintf(stderr, "Error: Invalid integer input\n");
        return EXIT_FAILURE;
    }

    if (value > 2147483647L || value < -2147483648L) {
        fprintf(stderr, "Error: Integer overflow\n");
        return EXIT_FAILURE;
    }

    int number = (int)value;

    if (is_even(number)) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }

    return EXIT_SUCCESS;
}