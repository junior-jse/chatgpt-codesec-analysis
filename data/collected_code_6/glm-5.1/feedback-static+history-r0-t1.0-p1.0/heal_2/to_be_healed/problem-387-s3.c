/* Repair history (previous round):
 * - Flawfinder strlen was removed from check_hex_even_odd() and now appears in main().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef enum {
    RESULT_EVEN,
    RESULT_ODD,
    RESULT_INVALID
} HexResult;

HexResult check_hex_even_odd(const char *hex_str, size_t max_len) {
    if (hex_str == NULL) {
        return RESULT_INVALID;
    }

    size_t len = 0;
    while (len < max_len && hex_str[len] != '\0') {
        len++;
    }
    if (len == 0 || (len == max_len && hex_str[len] != '\0')) {
        return RESULT_INVALID;
    }

    size_t i = 0;
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        if (len <= 2) {
            return RESULT_INVALID;
        }
        i = 2;
    }

    for (; i < len; ++i) {
        char c = tolower((unsigned char)hex_str[i]);
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            return RESULT_INVALID;
        }
    }

    char last_char = tolower((unsigned char)hex_str[len - 1]);

    if (last_char == '0' || last_char == '2' || last_char == '4' || last_char == '6' || last_char == '8' || last_char == 'a' || last_char == 'c' || last_char == 'e') {
        return RESULT_EVEN;
    } else {
        return RESULT_ODD;
    }
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    HexResult result = check_hex_even_odd(argv[1], strlen(argv[1]) + 1);

    if (result == RESULT_EVEN) {
        printf("Even\n");
    } else if (result == RESULT_ODD) {
        printf("Odd\n");
    } else {
        fprintf(stderr, "Invalid hexadecimal input\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}