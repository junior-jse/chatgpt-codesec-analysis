#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* minimizeString(const char* str, size_t str_len, char ch) {
    if (str == NULL) {
        return NULL;
    }

    size_t count = 0;

    for (size_t i = 0; i < str_len; i++) {
        if (str[i] == ch) {
            count++;
        }
    }

    if (count == str_len) {
        char* result = (char*)malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    char* result = (char*)malloc(str_len - count + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < str_len; i++) {
        if (str[i] != ch) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main() {
    char input[1024];
    char ch;

    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strnlen(input, sizeof(input));
    if (len == sizeof(input) && input[len - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    printf("Enter the character to remove: ");
    if (scanf(" %c", &ch) != 1) {
        fprintf(stderr, "Error reading character\n");
        return 1;
    }

    char* result = minimizeString(input, len, ch);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Minimized string: %s\n", result);

    free(result);
    result = NULL;

    return 0;
}