#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 256

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

bool is_hex_even(const char *hex_str, size_t max_len) {
    if (hex_str == NULL || max_len == 0) {
        return false;
    }

    size_t len = bounded_strlen(hex_str, max_len);
    if (len == 0 || len >= max_len) {
        return false;
    }

    size_t start = 0;

    if (len >= 2 && hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        start = 2;
    }

    if (start >= len) {
        return false;
    }

    char last_char = '\0';
    for (size_t i = start; i < len; i++) {
        if (!isxdigit((unsigned char)hex_str[i])) {
            return false;
        }
        last_char = hex_str[i];
    }

    int digit;
    if (last_char >= '0' && last_char <= '9') {
        digit = last_char - '0';
    } else if (last_char >= 'a' && last_char <= 'f') {
        digit = last_char - 'a' + 10;
    } else if (last_char >= 'A' && last_char <= 'F') {
        digit = last_char - 'A' + 10;
    } else {
        return false;
    }

    return (digit % 2) == 0;
}

int main(void) {
    char buffer[MAX_INPUT_SIZE] = {0};

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = bounded_strlen(buffer, sizeof(buffer));

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else if (len == sizeof(buffer) - 1) {
            int ch = getchar();
            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                printf("Odd\n");
                return 0;
            }
        }

        if (is_hex_even(buffer, sizeof(buffer))) {
            printf("Even\n");
        } else {
            printf("Odd\n");
        }
    }

    return 0;
}