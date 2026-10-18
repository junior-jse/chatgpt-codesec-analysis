#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 4096

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strnlen(input, MAX_INPUT_LENGTH + 1);
    if (len > MAX_INPUT_LENGTH) {
        return NULL;
    }

    if (len > (SIZE_MAX - 1) / 2) {
        return NULL;
    }

    size_t max_len = len * 2 + 1;
    char *output = malloc(max_len);
    if (output == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            if (i > 0) {
                output[j++] = '_';
            }
            output[j++] = (char)tolower((unsigned char)input[i]);
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';

    char *result = realloc(output, j + 1);
    if (result == NULL) {
        free(output);
        return NULL;
    }

    return result;
}

int main(void) {
    const char *test_cases[] = {
        "camelCaseString",
        "helloWorld",
        "a",
        "ABC",
        "alreadySnakeCase",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char *result = camel_to_snake(test_cases[i]);
        if (result != NULL) {
            printf("Input: %s\nOutput: %s\n\n", test_cases[i], result);
            free(result);
        } else {
            fprintf(stderr, "Conversion failed\n");
            return 1;
        }
    }

    return 0;
}