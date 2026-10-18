#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROMAN_LENGTH 16

char* int_to_roman(int num) {
    if (num < 1 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int num_symbols = sizeof(values) / sizeof(values[0]);

    char* roman = (char*)malloc(MAX_ROMAN_LENGTH * sizeof(char));
    if (roman == NULL) {
        return NULL;
    }

    roman[0] = '\0';
    int remaining = num;
    size_t current_len = 0;

    for (int i = 0; i < num_symbols; i++) {
        while (remaining >= values[i]) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t sym_len = strlen(symbols[i]);
            if (current_len + sym_len >= MAX_ROMAN_LENGTH) {
                free(roman);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(roman + current_len, symbols[i]);
            current_len += sym_len;
            remaining -= values[i];
        }
    }

    return roman;
}

int main(void) {
    int num = 1994;
    char* roman = int_to_roman(num);
    if (roman != NULL) {
        printf("%d -> %s\n", num, roman);
        free(roman);
    } else {
        printf("Invalid input or memory allocation failed.\n");
    }
    return 0;
}