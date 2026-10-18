#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int is_hexadecimal(const char *text)
{
    size_t index = 0;

    if (text == NULL || text[0] == '\0') {
        return 0;
    }

    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        index = 2;
    }

    if (text[index] == '\0') {
        return 0;
    }

    for (; text[index] != '\0'; ++index) {
        if (!isxdigit((unsigned char)text[index])) {
            return 0;
        }
    }

    return 1;
}

static int is_hex_even(const char *text)
{
    size_t length = strlen(text);
    char last_digit = text[length - 1];

    if (last_digit >= '0' && last_digit <= '9') {
        return ((last_digit - '0') % 2) == 0;
    }

    last_digit = (char)tolower((unsigned char)last_digit);
    return ((last_digit - 'a' + 10) % 2) == 0;
}

int main(void)
{
    char input[1024];

    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("Input error\n", stderr);
        return 1;
    }

    size_t length = strlen(input);

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (!feof(stdin)) {
        int character;

        do {
            character = getchar();
        } while (character != '\n' && character != EOF);

        fputs("Input too long\n", stderr);
        return 1;
    }

    if (!is_hexadecimal(input)) {
        fputs("Invalid hexadecimal number\n", stderr);
        return 1;
    }

    puts(is_hex_even(input) ? "Even" : "Odd");
    return 0;
}