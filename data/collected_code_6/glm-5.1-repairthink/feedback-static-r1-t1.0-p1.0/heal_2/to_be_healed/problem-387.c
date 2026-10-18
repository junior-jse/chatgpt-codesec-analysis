#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int is_hex_even(const char *hex_str) {
    size_t i;
    char last_char = 0;
    int value;
    int found_digit = 0;

    if (hex_str == NULL) {
        errno = EINVAL;
        return -1;
    }

    i = 0;
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        i = 2;
    }

    for (; hex_str[i] != '\0'; i++) {
        if (!isxdigit((unsigned char)hex_str[i])) {
            errno = EINVAL;
            return -1;
        }
        last_char = hex_str[i];
        found_digit = 1;
    }

    if (!found_digit) {
        errno = EINVAL;
        return -1;
    }

    last_char = (char)toupper((unsigned char)last_char);

    if (last_char >= '0' && last_char <= '9') {
        value = last_char - '0';
    } else {
        value = 10 + last_char - 'A';
    }

    return (value % 2 == 0) ? 1 : 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    int result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    result = is_hex_even(argv[1]);

    if (errno != 0) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (result == 1) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }

    return EXIT_SUCCESS;
}