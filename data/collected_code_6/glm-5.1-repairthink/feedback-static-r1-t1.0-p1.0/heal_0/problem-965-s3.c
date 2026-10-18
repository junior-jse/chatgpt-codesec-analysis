#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *camel_str) {
    if (camel_str == NULL) {
        return NULL;
    }

    size_t len = strlen(camel_str);
    if (len == 0) {
        char *empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t new_len = len + 1;
    for (size_t i = 1; i < len; i++) {
        if (isupper((unsigned char)camel_str[i])) {
            new_len++;
        }
    }

    char *snake_str = malloc(new_len);
    if (snake_str == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && isupper((unsigned char)camel_str[i])) {
            snake_str[j++] = '_';
            snake_str[j++] = tolower((unsigned char)camel_str[i]);
        } else {
            snake_str[j++] = tolower((unsigned char)camel_str[i]);
        }
    }
    snake_str[j] = '\0';

    return snake_str;
}

int main(void) {
    const char *inputs[] = {
        "camelCase",
        "CamelCase",
        "snake_case",
        "already_snake_case",
        "singleword",
        "",
        "HTTPRequest",
        "XMLParser"
    };

    size_t count = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < count; i++) {
        char *result = camel_to_snake(inputs[i]);
        if (result != NULL) {
            printf("Input: \"%s\" -> Output: \"%s\"\n", inputs[i], result);
            free(result);
        } else {
            printf("Input: \"%s\" -> Allocation failed\n", inputs[i]);
        }
    }

    char *null_test = camel_to_snake(NULL);
    if (null_test == NULL) {
        printf("Null input handled correctly.\n");
    }

    return 0;
}