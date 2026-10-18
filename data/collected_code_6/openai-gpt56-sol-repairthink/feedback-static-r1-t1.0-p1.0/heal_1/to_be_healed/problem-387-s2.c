#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_hex_even(const char *hex, int *is_even)
{
    const char *digit;
    size_t length;

    if (hex == NULL || is_even == NULL) {
        return 0;
    }

    while (isspace((unsigned char)*hex)) {
        ++hex;
    }

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex += 2;
    }

    length = strcspn(hex, "\r\n");
    while (length > 0U && isspace((unsigned char)hex[length - 1U])) {
        --length;
    }

    if (length == 0U) {
        return 0;
    }

    for (size_t i = 0U; i < length; ++i) {
        if (!isxdigit((unsigned char)hex[i])) {
            return 0;
        }
    }

    digit = &hex[length - 1U];
    *is_even = (*digit == '0' || *digit == '2' || *digit == '4' ||
                *digit == '6' || *digit == '8' || *digit == 'a' ||
                *digit == 'A' || *digit == 'c' || *digit == 'C' ||
                *digit == 'e' || *digit == 'E');

    return 1;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[1024];
    int even;

    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("Failed to read input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        fputs("Input is too long.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!is_hex_even(input, &even)) {
        fputs("Invalid hexadecimal number.\n", stderr);
        return EXIT_FAILURE;
    }

    puts(even ? "Even" : "Odd");
    return EXIT_SUCCESS;
}