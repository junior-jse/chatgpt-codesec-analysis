#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int is_even_hex(const char *hex_str) {
    if (!hex_str) return -1;

    const char *p = hex_str;

    if (*p == '-' || *p == '+')
        p++;

    if (*p == '\0')
        return -1;

    int last_digit = -1;

    while (*p) {
        unsigned char c = (unsigned char)*p;
        if (!isxdigit(c))
            return -1;

        if (c >= '0' && c <= '9')
            last_digit = c - '0';
        else if (c >= 'a' && c <= 'f')
            last_digit = c - 'a' + 10;
        else
            last_digit = c - 'A' + 10;

        p++;
    }

    if (last_digit == -1)
        return -1;

    return (last_digit % 2 == 0) ? 1 : 0;
}

int main(int argc, const char *argv[]) {
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