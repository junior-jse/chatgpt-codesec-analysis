#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int is_hex_even(const char *hex, int *is_even)
{
    size_t length;
    unsigned char last;

    if (hex == NULL || is_even == NULL) {
        return 0;
    }

    while (isspace((unsigned char)*hex)) {
        ++hex;
    }

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex += 2;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(hex);
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

    last = (unsigned char)hex[length - 1U];
    *is_even = (last == '0' || last == '2' || last == '4' ||
                last == '6' || last == '8' || last == 'a' ||
                last == 'c' || last == 'e' || last == 'A' ||
                last == 'C' || last == 'E');

    return 1;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[1024];
    int is_even;

    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("Input error\n", stderr);
        return 1;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        fputs("Input too long\n", stderr);
        return 1;
    }

    if (!is_hex_even(input, &is_even)) {
        fputs("Invalid hexadecimal number\n", stderr);
        return 1;
    }

    puts(is_even ? "Even" : "Odd");
    return 0;
}