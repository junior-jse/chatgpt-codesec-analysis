#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    static const struct {
        int value;
        const char* symbol;
        size_t length;
    } roman_map[] = {
        {1000, "M",  sizeof("M")  - 1},
        {900,  "CM", sizeof("CM") - 1},
        {500,  "D",  sizeof("D")  - 1},
        {400,  "CD", sizeof("CD") - 1},
        {100,  "C",  sizeof("C")  - 1},
        {90,   "XC", sizeof("XC") - 1},
        {50,   "L",  sizeof("L")  - 1},
        {40,   "XL", sizeof("XL") - 1},
        {10,   "X",  sizeof("X")  - 1},
        {9,    "IX", sizeof("IX") - 1},
        {5,    "V",  sizeof("V")  - 1},
        {4,    "IV", sizeof("IV") - 1},
        {1,    "I",  sizeof("I")  - 1}
    };
    const size_t num_symbols = sizeof(roman_map) / sizeof(roman_map[0]);

    size_t buffer_size = 16;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    size_t current_len = 0;
    size_t i;

    for (i = 0; i < num_symbols; i++) {
        while (num >= roman_map[i].value) {
            size_t symbol_len = roman_map[i].length;

            if (current_len + symbol_len + 1 > buffer_size) {
                size_t new_size = buffer_size * 2;
                char* temp = realloc(result, new_size);
                if (temp == NULL) {
                    free(result);
                    return NULL;
                }
                result = temp;
                buffer_size = new_size;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + current_len, roman_map[i].symbol, symbol_len);
            current_len += symbol_len;
            result[current_len] = '\0';

            num -= roman_map[i].value;
        }
    }

    return result;
}

int main(void) {
    int test_values[] = {1, 4, 9, 27, 48, 59, 93, 141, 163, 402, 575, 911, 1024, 3999};
    size_t num_tests = sizeof(test_values) / sizeof(test_values[0]);
    size_t i;

    for (i = 0; i < num_tests; i++) {
        char* roman = int_to_roman(test_values[i]);
        if (roman != NULL) {
            printf("%d: %s\n", test_values[i], roman);
            free(roman);
        } else {
            printf("%d: Invalid input\n", test_values[i]);
        }
    }

    return 0;
}