#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    size_t max_len = len * 2 + 1;
    char *result = (char *)malloc(max_len);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            if (i > 0 && j > 0 && result[j - 1] != '_') {
                result[j++] = '_';
            }
            result[j++] = tolower((unsigned char)input[i]);
        } else {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char *test_cases[] = {
        "camelCase",
        "CamelCase",
        "already_snake",
        "HTTPServer",
        "",
        "a",
        "A",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char *snake = camel_to_snake(test_cases[i]);
        if (snake != NULL) {
            printf("%s -> %s\n", test_cases[i], snake);
            free(snake);
        } else {
            printf("%s -> (null)\n", test_cases[i]);
        }
    }

    return 0;
}