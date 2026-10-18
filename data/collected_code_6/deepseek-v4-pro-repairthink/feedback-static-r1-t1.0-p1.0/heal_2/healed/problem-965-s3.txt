#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static size_t safe_strlen(const char *str, size_t max) {
    size_t len = 0;
    while (len < max && str[len] != '\0') {
        len++;
    }
    return len;
}

char *camel_to_snake(const char *input, size_t len) {
    if (input == NULL || len == 0) {
        return NULL;
    }

    size_t max_output = len * 2 + 1;
    char *result = (char *)malloc(max_output);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < len && input[i] != '\0') {
        if (isupper((unsigned char)input[i])) {
            if (i > 0 && input[i-1] != '_' && j > 0 && result[j-1] != '_') {
                result[j++] = '_';
            }
            result[j++] = (char)tolower((unsigned char)input[i]);
        } else {
            result[j++] = input[i];
        }
        i++;
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char *test_cases[] = {
        "camelCase",
        "PascalCase",
        "camel",
        "",
        "already_snake",
        "HTTPResponse",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        size_t len = safe_strlen(test_cases[i], 1024);
        char *converted = camel_to_snake(test_cases[i], len);
        if (converted != NULL) {
            printf("'%s' -> '%s'\n", test_cases[i], converted);
            free(converted);
        } else {
            printf("'%s' -> (null)\n", test_cases[i]);
        }
    }

    return 0;
}