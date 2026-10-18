#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define INITIAL_INPUT_CAPACITY 64

static size_t bounded_strlen(const char *s, size_t max_len)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

bool is_hex_even(const char *hex_str, size_t max_len, bool *is_valid)
{
    size_t len;
    size_t start_idx;
    char last_char;
    int last_digit;

    if (hex_str == NULL || is_valid == NULL || max_len == 0) {
        return false;
    }

    *is_valid = false;

    len = bounded_strlen(hex_str, max_len);
    if (len == 0) {
        return false;
    }

    start_idx = 0;

    if (len > 2 && hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        start_idx = 2;
    }

    if (start_idx >= len) {
        return false;
    }

    for (size_t i = start_idx; i < len; i++) {
        if (!isxdigit((unsigned char)hex_str[i])) {
            return false;
        }
    }

    last_char = hex_str[len - 1];

    if (last_char >= '0' && last_char <= '9') {
        last_digit = last_char - '0';
    } else if (last_char >= 'A' && last_char <= 'F') {
        last_digit = last_char - 'A' + 10;
    } else if (last_char >= 'a' && last_char <= 'f') {
        last_digit = last_char - 'a' + 10;
    } else {
        return false;
    }

    *is_valid = true;
    return (last_digit % 2 == 0);
}

static char *read_input_line(FILE *stream)
{
    size_t capacity = INITIAL_INPUT_CAPACITY;
    size_t length = 0;
    char *buffer;
    int ch;

    if (stream == NULL) {
        return NULL;
    }

    buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((ch = fgetc(stream)) != EOF && ch != '\n') {
        if (length + 1 >= capacity) {
            size_t new_capacity;
            char *new_buffer;

            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }

            new_capacity = capacity * 2;
            new_buffer = realloc(buffer, new_capacity);

            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        buffer[length] = (char)ch;
        length++;
    }

    if (ferror(stream) || (ch == EOF && length == 0)) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void)
{
    char *input;
    size_t input_len;
    bool is_valid = false;
    bool is_even;

    printf("Enter a hexadecimal number: ");

    input = read_input_line(stdin);
    if (input == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);

    is_even = is_hex_even(input, input_len + 1, &is_valid);

    if (!is_valid) {
        fprintf(stderr, "Invalid hexadecimal number\n");
        free(input);
        return 1;
    }

    if (is_even) {
        printf("The hexadecimal number is even\n");
    } else {
        printf("The hexadecimal number is odd\n");
    }

    free(input);
    return 0;
}