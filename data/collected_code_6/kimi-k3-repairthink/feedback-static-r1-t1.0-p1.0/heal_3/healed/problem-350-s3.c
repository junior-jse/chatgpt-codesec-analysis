#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

char* minimizeString(const char* str, size_t str_size, char ch) {
    if (str == NULL || str_size == 0) {
        return NULL;
    }

    size_t len = strnlen(str, str_size);
    if (len == str_size) {
        return NULL;
    }

    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == ch) {
            count++;
        }
    }

    if (count == len) {
        char* result = (char*)malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    char* result = (char*)malloc(len - count + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != ch) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    char* input = NULL;
    size_t input_capacity = 0;
    ssize_t input_len;
    int c;
    int extra;

    printf("Enter a string: ");
    input_len = getline(&input, &input_capacity, stdin);
    if (input_len < 0) {
        fprintf(stderr, "Error reading input\n");
        free(input);
        return 1;
    }

    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    printf("Enter the character to remove: ");
    c = getchar();
    if (c == EOF || c == '\n') {
        fprintf(stderr, "Error reading character\n");
        free(input);
        return 1;
    }

    extra = getchar();
    if (extra != '\n' && extra != EOF) {
        fprintf(stderr, "Error: expected a single character\n");
        free(input);
        return 1;
    }

    size_t input_str_len = strnlen(input, (size_t)input_len + 1);
    char* result = minimizeString(input, input_str_len + 1, (char)c);
    if (result == NULL) {
        fprintf(stderr, "Failed to minimize string\n");
        free(input);
        return 1;
    }

    printf("Minimized string: %s\n", result);

    free(result);
    result = NULL;
    free(input);
    input = NULL;

    return 0;
}