#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 4096

char* camel_to_snake(const char* input, size_t input_size) {
    if (input == NULL || input_size == 0) {
        return NULL;
    }

    size_t len = strnlen(input, input_size);
    if (len == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    if (len > (SIZE_MAX - 1) / 2) {
        return NULL;
    }

    size_t result_size = len * 2 + 1;
    char* result = malloc(result_size);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            if (i > 0) {
                result[j++] = '_';
            }
            result[j++] = (char)tolower((unsigned char)input[i]);
        } else {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char* test_cases[] = {
        "camelCaseString",
        "simpleTest",
        "ABC",
        "already_snake",
        "a",
        ""
    };

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char* snake = camel_to_snake(test_cases[i], MAX_INPUT_LENGTH);
        if (snake != NULL) {
            printf("Input: %s\n", test_cases[i]);
            printf("Output: %s\n\n", snake);
            free(snake);
        } else {
            fprintf(stderr, "Conversion failed for: %s\n", test_cases[i]);
        }
    }

    return 0;
}