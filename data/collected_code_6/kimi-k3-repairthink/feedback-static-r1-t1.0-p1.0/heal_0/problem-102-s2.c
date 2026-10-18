#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* snake_to_camel(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    int capitalize_next = 0;

    while (i < len) {
        if (input[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                result[j++] = (char)toupper((unsigned char)input[i]);
                capitalize_next = 0;
            } else {
                result[j++] = input[i];
            }
        }
        i++;
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char* test_cases[] = {
        "hello_world",
        "snake_case_string",
        "single",
        "multiple__underscores",
        "_leading_underscore",
        "trailing_underscore_",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char* camel = snake_to_camel(test_cases[i]);
        if (camel != NULL) {
            printf("Input:  %s\n", test_cases[i]);
            printf("Output: %s\n\n", camel);
            free(camel);
        } else {
            fprintf(stderr, "Memory allocation failed for input: %s\n", test_cases[i]);
        }
    }

    return 0;
}