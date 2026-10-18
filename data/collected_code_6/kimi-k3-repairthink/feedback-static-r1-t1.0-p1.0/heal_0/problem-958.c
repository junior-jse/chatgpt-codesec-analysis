#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }
    
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const size_t num_symbols = sizeof(values) / sizeof(values[0]);
    
    size_t buffer_size = 16;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';
    
    size_t current_len = 0;
    size_t i;
    
    for (i = 0; i < num_symbols; i++) {
        while (num >= values[i]) {
            size_t symbol_len = strlen(symbols[i]);
            
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
            
            strcat(result, symbols[i]);
            current_len += symbol_len;
            num -= values[i];
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