#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024

char *snake_to_camel(const char *input) {
    if (!input) {
        return NULL;
    }

    size_t len = 0;
    while (len < MAX_INPUT_LENGTH && input[len] != '\0') {
        len++;
    }
    if (len == MAX_INPUT_LENGTH) {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (!result) {
        return NULL;
    }

    size_t j = 0;
    int capitalize_next = 0;

    for (size_t i = 0; i < len; i++) {
        if (input[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next && input[i] >= 'a' && input[i] <= 'z') {
                result[j++] = input[i] - 32;
            } else {
                result[j++] = input[i];
            }
            capitalize_next = 0;
        }
    }

    result[j] = '\0';
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "hello_world",
        "snake_case_string",
        "convert_this_to_camel",
        "already_camel",
        "multiple__underscores",
        "_leading_underscore",
        "trailing_underscore_",
        "numbers_123_test",
        "all_lower_case",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char *converted = snake_to_camel(test_cases[i]);
        if (converted) {
            printf("%s -> %s\n", test_cases[i], converted);
            free(converted);
        } else {
            printf("%s -> NULL\n", test_cases[i]);
        }
    }

    return 0;
}