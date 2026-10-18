#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *integer_to_roman(unsigned int value)
{
    static const struct {
        unsigned int value;
        const char *symbol;
    } numerals[] = {
        {1000U, "M"},
        {900U, "CM"},
        {500U, "D"},
        {400U, "CD"},
        {100U, "C"},
        {90U, "XC"},
        {50U, "L"},
        {40U, "XL"},
        {10U, "X"},
        {9U, "IX"},
        {5U, "V"},
        {4U, "IV"},
        {1U, "I"}
    };

    char *result;
    char *position;
    size_t remaining;
    size_t i;

    if (value == 0U || value > 3999U) {
        return NULL;
    }

    result = malloc(16U);
    if (result == NULL) {
        return NULL;
    }

    position = result;
    remaining = 16U;

    for (i = 0U; i < sizeof(numerals) / sizeof(numerals[0]); ++i) {
        size_t symbol_length = strlen(numerals[i].symbol);

        while (value >= numerals[i].value) {
            if (symbol_length >= remaining) {
                free(result);
                return NULL;
            }

            memcpy(position, numerals[i].symbol, symbol_length);
            position += symbol_length;
            remaining -= symbol_length;
            value -= numerals[i].value;
        }
    }

    *position = '\0';
    return result;
}

int main(void)
{
    int input;
    char *roman;

    if (scanf("%d", &input) != 1) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (input < 1 || input > 3999) {
        fputs("Input must be between 1 and 3999\n", stderr);
        return EXIT_FAILURE;
    }

    roman = integer_to_roman((unsigned int)input);
    if (roman == NULL) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", roman) < 0) {
        free(roman);
        return EXIT_FAILURE;
    }

    free(roman);
    return EXIT_SUCCESS;
}