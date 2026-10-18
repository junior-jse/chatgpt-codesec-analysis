#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const size_t symbol_lens[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
    const int num_symbols = 13;
    
    char *result = (char *)malloc(16 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }
    
    result[0] = '\0';
    size_t current_len = 0;
    int remaining = num;
    
    for (int i = 0; i < num_symbols && remaining > 0; i++) {
        while (remaining >= values[i]) {
            size_t symbol_len = symbol_lens[i];
            if (current_len + symbol_len + 1 > 16) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + current_len, symbols[i], symbol_len);
            current_len += symbol_len;
            result[current_len] = '\0';
            remaining -= values[i];
        }
    }
    
    return result;
}

int main(void) {
    int test_values[] = {0, 1, 4, 9, 58, 1994, 3549, 3999, 4000, -5};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);
    
    for (int i = 0; i < num_tests; i++) {
        char *roman = int_to_roman(test_values[i]);
        if (roman != NULL) {
            printf("%d -> %s\n", test_values[i], roman);
            free(roman);
        } else {
            printf("%d -> Invalid input (must be between 1 and 3999)\n", test_values[i]);
        }
    }
    
    return 0;
}