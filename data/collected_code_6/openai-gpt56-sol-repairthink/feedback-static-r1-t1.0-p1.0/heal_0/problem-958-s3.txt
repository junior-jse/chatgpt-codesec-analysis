#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *integer_to_roman(unsigned int value)
{
    static const struct {
        unsigned int value;
        const char *symbol;
    } numerals[] = {
        {1000U, "M"}, {900U, "CM"}, {500U, "D"}, {400U, "CD"},
        {100U, "C"}, {90U, "XC"}, {50U, "L"}, {40U, "XL"},
        {10U, "X"}, {9U, "IX"}, {5U, "V"}, {4U, "IV"},
        {1U, "I"}
    };
    char buffer[16];
    size_t position = 0U;
    size_t i;
    char *result;

    if (value == 0U || value > 3999U) {
        return NULL;
    }

    for (i = 0U; i < sizeof(numerals) / sizeof(numerals[0]); ++i) {
        while (value >= numerals[i].value) {
            size_t length = strlen(numerals[i].symbol);

            if (position + length >= sizeof(buffer)) {
                return NULL;
            }

            memcpy(buffer + position, numerals[i].symbol, length);
            position += length;
            value -= numerals[i].value;
        }
    }

    buffer[position] = '\0';

    result = malloc(position + 1U);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, buffer, position + 1U);
    return result;
}

int main(void)
{
    unsigned int value;
    char trailing;
    char *roman;

    if (scanf("%u%c", &value, &trailing) != 2 ||
        (trailing != '\n' && trailing != EOF)) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    roman = integer_to_roman(value);
    if (roman == NULL) {
        fputs("Value must be between 1 and 3999\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", roman) < 0) {
        free(roman);
        return EXIT_FAILURE;
    }

    free(roman);
    return EXIT_SUCCESS;
}