/* Repair history (previous round):
 * - Flawfinder memcpy was replaced by strncpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_to_roman(int num, char *result, size_t size) {
    if (num <= 0 || num > 3999 || result == NULL || size < 16) {
        if (result != NULL && size > 0) {
            result[0] = '\0';
        }
        return;
    }

    const char *roman_numerals[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    const size_t roman_lengths[] = {
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1
    };
    const int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };

    result[0] = '\0';
    size_t offset = 0;

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            size_t len = roman_lengths[i];
            if (offset + len + 1 > size) {
                result[0] = '\0';
                return;
            }
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(result + offset, roman_numerals[i], size - offset - 1);
            offset += len;
            result[offset] = '\0';
            num -= values[i];
        }
    }
}

int main(void) {
    int number;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char roman[64];

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