#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int is_hex_even(const char *hex_str) {
    if (hex_str == NULL) {
        return -1;
    }

    size_t len = strlen(hex_str);
    if (len == 0) {
        return -1;
    }

    size_t start = 0;
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        start = 2;
    }

    if (start >= len) {
        return -1;
    }

    size_t last_idx = len - 1;
    char last_char = tolower((unsigned char)hex_str[last_idx]);

    if ((last_char >= '0' && last_char <= '9') || (last_char >= 'a' && last_char <= 'f')) {
        return (last_char - '0') % 2 == 0 ? 1 : 0;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int result = is_hex_even(argv[1]);
    if (result == -1) {
        fprintf(stderr, "Invalid hexadecimal input\n");
        return EXIT_FAILURE;
    }

    if (result == 1) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }

    return EXIT_SUCCESS;
}