#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

char *read_line(void) {
    size_t capacity = 128;
    size_t length = 0;
    char *buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }
    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_buffer = realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[length++] = (char)c;
    }
    if (c == EOF && length == 0) {
        free(buffer);
        return NULL;
    }
    buffer[length] = '\0';
    return buffer;
}

bool is_hex_even(const char *hex_str) {
    if (hex_str == NULL) {
        return false;
    }
    size_t len = 0;
    while (hex_str[len] != '\0') {
        len++;
    }
    if (len == 0) {
        return false;
    }
    if (hex_str[len - 1] == '\n') {
        len--;
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
    char *input = read_line();
    if (input != NULL) {
        if (is_hex_even(input)) {
            printf("Even\n");
        } else {
            printf("Odd\n");
        }
        free(input);
    }
    return 0;
}