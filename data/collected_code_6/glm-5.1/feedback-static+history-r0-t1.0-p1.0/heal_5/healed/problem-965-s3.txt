#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char *camel_to_snake(const char *camel_str, size_t camel_len) {
    if (camel_str == NULL) {
        return NULL;
    }

    if (camel_len == 0) {
        char *empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t new_len = camel_len + 1;
    for (size_t i = 1; i < camel_len; i++) {
        if (isupper((unsigned char)camel_str[i])) {
            new_len++;
        }
    }

    char *snake_str = malloc(new_len);
    if (snake_str == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < camel_len; i++) {
        if (i > 0 && isupper((unsigned char)camel_str[i])) {
            snake_str[j++] = '_';
        }
        if (isupper((unsigned char)camel_str[i])) {
            snake_str[j++] = tolower((unsigned char)camel_str[i]);
        } else {
            snake_str[j++] = camel_str[i];
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
        size_t len = strnlen(inputs[i], 256);
        char *result = camel_to_snake(inputs[i], len);
        if (result != NULL) {
            printf("Input: \"%s\" -> Output: \"%s\"\n", inputs[i], result);
            free(result);
        } else {
            printf("Input: \"%s\" -> Allocation failed\n", inputs[i]);
        }
    }

    char *null_test = camel_to_snake(NULL, 0);
    if (null_test == NULL) {
        printf("Null input handled correctly.\n");
    } else {
        free(null_test);
    }

    size_t zero_len_test_len = 0;
    char *zero_len_test = camel_to_snake("ignored", zero_len_test_len);
    if (zero_len_test != NULL && zero_len_test[0] == '\0') {
        printf("Zero length input handled correctly.\n");
        free(zero_len_test);
    } else if (zero_len_test != NULL) {
        free(zero_len_test);
    }

    return 0;
}