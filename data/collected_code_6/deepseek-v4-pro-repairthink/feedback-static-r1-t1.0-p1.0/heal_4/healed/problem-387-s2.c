#include <stdio.h>
#include <stdbool.h>

#define INPUT_SIZE 4096

bool is_hex_even(const char *hex_str, size_t max_len) {
    if (hex_str == NULL || max_len == 0) {
        return false;
    }

    size_t len = 0;
    while (len < max_len && hex_str[len] != '\0') {
        ++len;
    }

    if (len == max_len) {
        return false;
    }

    if (len == 0) {
        return false;
    }

    if (hex_str[len - 1] == '\n') {
        --len;
    }

    if (len == 0) {
        return false;
    }

    char last_char = hex_str[len - 1];

    switch (last_char) {
        case '0': case '2': case '4': case '6': case '8':
        case 'A': case 'C': case 'E':
        case 'a': case 'c': case 'e':
            return true;
        case '1': case '3': case '5': case '7': case '9':
        case 'B': case 'D': case 'F':
        case 'b': case 'd': case 'f':
            return false;
        default:
            return false;
    }
}

int main(void) {
    char input[INPUT_SIZE];

    if (fgets(input, INPUT_SIZE, stdin) != NULL) {
        if (is_hex_even(input, INPUT_SIZE)) {
            printf("Even\n");
        } else {
            printf("Odd\n");
        }
    }

    return 0;
}