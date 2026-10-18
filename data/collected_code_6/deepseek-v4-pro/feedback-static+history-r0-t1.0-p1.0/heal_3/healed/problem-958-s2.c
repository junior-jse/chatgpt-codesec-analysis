#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const size_t symbol_lengths[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};

    char *result = malloc(16 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t current_len = 0;
    int i = 0;
    int remaining = num;

    while (remaining > 0) {
        while (remaining >= values[i]) {
            size_t sym_len = symbol_lengths[i];
            if (current_len + sym_len >= 15) {
                free(result);
                return NULL;
            }
            memcpy(result + current_len, symbols[i], sym_len);
            current_len += sym_len;
            result[current_len] = '\0';
            remaining -= values[i];
        }
        i++;
    }

    return result;
}

int main(void) {
    int test_values[] = {1, 3, 4, 9, 40, 90, 400, 900, 1994, 2023, 3999, 0, -5, 4000};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("%d -> ", test_values[i]);
        char *roman = int_to_roman(test_values[i]);
        if (roman != NULL) {
            printf("%s\n", roman);
            free(roman);
        } else {
            printf("Invalid input (must be 1-3999)\n");
        }
    }

    return 0;
}