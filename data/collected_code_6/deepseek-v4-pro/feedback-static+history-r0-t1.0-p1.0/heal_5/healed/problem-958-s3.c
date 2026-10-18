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

    const char symbols[13][3] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const size_t symbol_lengths[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
    
    result[0] = '\0';
    int remaining = num;
    size_t current_length = 0;
    size_t max_index = result_size - 1;
    
    for (int i = 0; i < 13; i++) {
        while (remaining >= values[i]) {
            size_t symbol_length = symbol_lengths[i];
            if (current_length + symbol_length < result_size) {
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
        char roman[ROMAN_MAX_LENGTH] = {0};
        int_to_roman(test_numbers[i], roman, ROMAN_MAX_LENGTH);
        printf("%d = %s\n", test_numbers[i], roman);
    }
    
    return 0;
}