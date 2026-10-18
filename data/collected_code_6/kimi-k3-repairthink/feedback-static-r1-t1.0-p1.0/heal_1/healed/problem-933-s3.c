#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 4096

char* camel_to_snake(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len == MAX_INPUT_LEN) {
        return NULL;
    }

    size_t output_size = input_len * 2 + 1;
    char* output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    size_t out_idx = 0;
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (isupper(c)) {
            if (i > 0) {
                output[out_idx++] = '_';
            }
            output[out_idx++] = (char)tolower(c);
        } else {
            output[out_idx++] = (char)c;
        }
    }
    output[out_idx] = '\0';

    return output;
}

int main(void) {
    const char* test_strings[] = {
        "camelCaseString",
        "thisIsATest",
        "helloWorld",
        "simpleTest",
        "ABCDefGhi"
    };

    size_t num_tests = sizeof(test_strings) / sizeof(test_strings[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char* result = camel_to_snake(test_strings[i]);
        if (result != NULL) {
            printf("Input:  %s\n", test_strings[i]);
            printf("Output: %s\n\n", result);
            free(result);
        } else {
            fprintf(stderr, "Error converting: %s\n", test_strings[i]);
        }
    }

    return 0;
}