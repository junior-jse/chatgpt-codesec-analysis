#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

char* camel_to_snake(const char* input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    if (len == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t result_size = len * 2 + 1;
    char* result = malloc(result_size);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)input[i])) {
            if (i > 0) {
                result[j++] = '_';
            }
            result[j++] = tolower((unsigned char)input[i]);
        } else {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';

    regfree(&regex);
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
        char* snake = camel_to_snake(test_cases[i]);
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