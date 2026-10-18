#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int is_hex_even(const char *hex_str, size_t max_len) {
    size_t len;
    size_t i;
    int last_char;
    int value;

    if (hex_str == NULL) {
        errno = EINVAL;
        return -1;
    }

    len = strnlen(hex_str, max_len);
    if (len == 0 || len == max_len) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        if (!isxdigit((unsigned char)hex_str[i])) {
            errno = EINVAL;
            return -1;
        }
    }

    if (hex_str[0] == '0' && len > 1 && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        if (len == 2) {
            errno = EINVAL;
            return -1;
        }
        last_char = toupper((unsigned char)hex_str[len - 1]);
    } else {
        last_char = toupper((unsigned char)hex_str[len - 1]);
    }

    switch (last_char) {
        case '0': value = 0; break;
        case '1': value = 1; break;
        case '2': value = 2; break;
        case '3': value = 3; break;
        case '4': value = 4; break;
        case '5': value = 5; break;
        case '6': value = 6; break;
        case '7': value = 7; break;
        case '8': value = 8; break;
        case '9': value = 9; break;
        case 'A': value = 10; break;
        case 'B': value = 11; break;
        case 'C': value = 12; break;
        case 'D': value = 13; break;
        case 'E': value = 14; break;
        case 'F': value = 15; break;
        default:
            errno = EINVAL;
            return -1;
    }

    return (value % 2 == 0) ? 1 : 0;
}

int main(int argc, const char * const argv[]) {
    int result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    errno = 0;
    result = is_hex_even(argv[1], 4096);

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