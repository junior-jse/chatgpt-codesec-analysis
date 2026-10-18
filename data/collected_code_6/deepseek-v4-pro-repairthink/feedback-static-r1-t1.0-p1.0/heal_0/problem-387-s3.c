#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_even_hex(const char *hex_str) {
    if (hex_str == NULL || *hex_str == '\0') {
        return -1;
    }

    size_t len = strlen(hex_str);
    size_t i = 0;

    if (hex_str[0] == '-' || hex_str[0] == '+') {
        i = 1;
    }

    if (i >= len) {
        return -1;
    }

    for (size_t j = i; j < len; j++) {
        if (!isxdigit((unsigned char)hex_str[j])) {
            return -1;
        }
    }

    char last_char = hex_str[len - 1];
    int last_digit;

    if (last_char >= '0' && last_char <= '9') {
        last_digit = last_char - '0';
    } else if (last_char >= 'a' && last_char <= 'f') {
        last_digit = last_char - 'a' + 10;
    } else {
        last_digit = last_char - 'A' + 10;
    }

    return (last_digit % 2 == 0) ? 1 : 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_number>\n", argv[0]);
        return 1;
    }

    int result = is_even_hex(argv[1]);
    
    if (result == -1) {
        fprintf(stderr, "Invalid hexadecimal number\n");
        return 1;
    }

    printf("%s\n", result ? "even" : "odd");
    return 0;
}