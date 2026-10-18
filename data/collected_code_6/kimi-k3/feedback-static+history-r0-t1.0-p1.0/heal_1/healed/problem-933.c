#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LEN 1024

char *camel_to_snake(const char *input, size_t max_len) {
    if (input == NULL || max_len == 0) {
        return NULL;
    }

    size_t input_len = strnlen(input, max_len);
    if (input_len == 0 || input_len >= MAX_STRING_LEN) {
        return NULL;
    }

    size_t output_len = input_len * 2 + 1;
    char *output = (char *)malloc(output_len);
    if (output == NULL) {
        return NULL;
    }

    size_t out_idx = 0;
    int first_char = 1;

    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        
        if (isupper(c)) {
            if (!first_char) {
                output[out_idx++] = '_';
            }
            output[out_idx++] = tolower(c);
            first_char = 0;
        } else {
            output[out_idx++] = tolower(c);
        }
    }

    output[out_idx] = '\0';

    if (out_idx == 0) {
        free(output);
        return NULL;
    }

    return output;
}

int main(void) {
    const char *test_cases[] = {
        "camelCaseString",
        "anotherExampleHere",
        "simpleTest",
        "XMLHttpRequest",
        "already_snake_case",
        "lowercase",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char *result = camel_to_snake(test_cases[i], MAX_STRING_LEN);
        if (result != NULL) {
            printf("Input:  %s\n", test_cases[i]);
            printf("Output: %s\n\n", result);
            free(result);
        } else {
            printf("Failed to convert: %s\n\n", test_cases[i]);
        }
    }

    return 0;
}