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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[16];
    size_t position = 0U;
    size_t i;
    char *result;

    if (value == 0U || value > 3999U) {
        return NULL;
    }

    for (i = 0U; i < sizeof(numerals) / sizeof(numerals[0]); ++i) {
        while (value >= numerals[i].value) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t length = strlen(numerals[i].symbol);

            if (position + length >= sizeof(buffer)) {
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
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

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
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