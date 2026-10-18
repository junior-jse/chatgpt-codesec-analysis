/* Repair history (previous round):
 * - Flawfinder strcat was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

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
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t sym_len = strlen(symbols[i]);
            if (current_len + sym_len >= 16) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + current_len, symbols[i], sym_len + 1);
            current_len += sym_len;
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