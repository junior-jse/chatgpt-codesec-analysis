#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROMAN_MAX_LENGTH 16

void int_to_roman(int num, char *result, size_t result_size) {
    if (result == NULL || result_size == 0) {
        return;
    }

    if (num <= 0 || num > 3999) {
        result[0] = '\0';
        return;
    }

    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    
    result[0] = '\0';
    int remaining = num;
    size_t current_length = 0;
    size_t max_index = result_size - 1;
    
    for (int i = 0; i < 13; i++) {
        while (remaining >= values[i]) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t symbol_length = strlen(symbols[i]);
            if (current_length + symbol_length <= max_index) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + current_length, symbols[i], symbol_length);
                current_length += symbol_length;
                result[current_length] = '\0';
            } else {
                return;
            }
            remaining -= values[i];
        }
    }
}

int main(void) {
    int test_numbers[] = {1, 3, 4, 9, 58, 1994, 3999};
    int num_tests = sizeof(test_numbers) / sizeof(test_numbers[0]);
    
    for (int i = 0; i < num_tests; i++) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char roman[ROMAN_MAX_LENGTH] = {0};
        int_to_roman(test_numbers[i], roman, sizeof(roman));
        printf("%d = %s\n", test_numbers[i], roman);
    }
    
    return 0;
}