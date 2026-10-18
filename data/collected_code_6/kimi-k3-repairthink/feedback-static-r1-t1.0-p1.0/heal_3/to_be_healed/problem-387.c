#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

int is_hex_even(const char *hex, size_t max_len) {
    if (hex == NULL || max_len == 0) {
        return -1;
    }

    size_t len = strnlen(hex, max_len);
    if (len == 0 || len >= max_len) {
        return -1;
    }

    size_t start = 0;

    if (len > 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        start = 2;
    }

    if (start >= len) {
        return -1;
    }

    for (size_t i = start; i < len; i++) {
        if (!isxdigit((unsigned char)hex[i])) {
            return -1;
        }
    }

    char last = hex[len - 1];
    int digit;

    if (last >= '0' && last <= '9') {
        digit = last - '0';
    } else if (last >= 'a' && last <= 'f') {
        digit = last - 'a' + 10;
    } else if (last >= 'A' && last <= 'F') {
        digit = last - 'A' + 10;
    } else {
        return -1;
    }

    return (digit % 2 == 0) ? 1 : 0;
}

static char *read_line(FILE *stream, size_t *out_len) {
    size_t capacity = 64;
    size_t length = 0;
    int c;

    char *buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }
            size_t new_capacity = capacity * 2;
            char *new_buffer = realloc(buffer, new_capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
            capacity = new_capacity;
        }
        buffer[length++] = (char)c;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';

    if (out_len != NULL) {
        *out_len = length;
    }

    return buffer;
}

int main(void) {
    size_t input_len = 0;
    char *buffer = read_line(stdin, &input_len);

    if (buffer == NULL) {
        return 1;
    }

    int result = is_hex_even(buffer, input_len + 1);

    free(buffer);

    if (result == 1) {
        printf("Even\n");
    } else if (result == 0) {
        printf("Odd\n");
    } else {
        printf("Invalid hexadecimal number\n");
    }

    return 0;
}