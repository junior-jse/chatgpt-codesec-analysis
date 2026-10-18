#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *integer_to_roman(unsigned int value)
{
    static const struct {
        unsigned int value;
        const char *symbol;
    } numerals[] = {
        {1000U, "M"}, {900U, "CM"}, {500U, "D"}, {400U, "CD"},
        {100U, "C"},  {90U, "XC"},  {50U, "L"},  {40U, "XL"},
        {10U, "X"},   {9U, "IX"},   {5U, "V"},   {4U, "IV"},
        {1U, "I"}
    };
    char *result;
    size_t position = 0U;

    if (value == 0U || value > 3999U) {
        errno = EDOM;
        return NULL;
    }

    result = malloc(16U);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0U; i < sizeof(numerals) / sizeof(numerals[0]); ++i) {
        while (value >= numerals[i].value) {
            size_t length = strlen(numerals[i].symbol);

            memcpy(result + position, numerals[i].symbol, length);
            position += length;
            value -= numerals[i].value;
        }
    }

    result[position] = '\0';
    return result;
}

int main(void)
{
    unsigned int value;
    char extra;
    char *roman;

    printf("Enter an integer from 1 to 3999: ");
    if (scanf("%u%c", &value, &extra) != 2 || extra != '\n') {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    roman = integer_to_roman(value);
    if (roman == NULL) {
        if (errno == EDOM) {
            fprintf(stderr, "Value must be from 1 to 3999\n");
        } else {
            perror("Unable to convert value");
        }
        return EXIT_FAILURE;
    }

    if (printf("%s\n", roman) < 0) {
        free(roman);
        return EXIT_FAILURE;
    }

    free(roman);
    return EXIT_SUCCESS;
}