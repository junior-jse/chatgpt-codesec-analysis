#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 4096

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
    if (stream == NULL) {
        return NULL;
    }

    char *buffer = malloc(MAX_LINE_LENGTH);
    if (buffer == NULL) {
        return NULL;
    }

    if (fgets(buffer, MAX_LINE_LENGTH, stream) == NULL) {
        free(buffer);
        return NULL;
    }

    if (ferror(stream)) {
        free(buffer);
        return NULL;
    }

    size_t length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else if (length == MAX_LINE_LENGTH - 1) {
        int c = fgetc(stream);
        if (c != EOF && c != '\n') {
            while ((c = fgetc(stream)) != EOF && c != '\n') {
            }
            free(buffer);
            return NULL;
        }
    }

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