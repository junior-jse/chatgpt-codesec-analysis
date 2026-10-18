#include <stdio.h>
#include <stdlib.h>

#define ROMAN_NUMERAL_MAX_LEN 15
#define ROMAN_BUFFER_SIZE 32

void int_to_roman(int num, char *result, size_t size) {
    if (result == NULL || size == 0) {
        return;
    }

    result[0] = '\0';

    if (num <= 0 || num > 3999 || size <= ROMAN_NUMERAL_MAX_LEN) {
        return;
    }

    static const char *const roman_numerals[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    static const int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };

    size_t offset = 0;
    const size_t count = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < count; i++) {
        while (num >= values[i]) {
            size_t remaining = size - offset;
            int written = snprintf(result + offset, remaining, "%s", roman_numerals[i]);
            if (written < 0 || (size_t)written >= remaining) {
                result[0] = '\0';
                return;
            }
            offset += (size_t)written;
            num -= values[i];
        }
    }
}

int main(void) {
    int number;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char roman[ROMAN_BUFFER_SIZE];

    printf("Enter an integer (1-3999): ");
    if (scanf("%d", &number) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    int_to_roman(number, roman, sizeof(roman));

    if (roman[0] == '\0') {
        fprintf(stderr, "Number out of range or conversion failed.\n");
        return EXIT_FAILURE;
    }

    printf("Roman numeral: %s\n", roman);
    return EXIT_SUCCESS;
}