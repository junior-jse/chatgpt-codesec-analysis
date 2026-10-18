#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

#define MAX_STRING_LEN 1024

char *camel_to_snake(const char *input) {
    if (input == NULL || *input == '\0') {
        return NULL;
    }

    size_t input_len = strlen(input);
    if (input_len >= MAX_STRING_LEN) {
        return NULL;
    }

    size_t output_len = input_len * 2 + 1;
    char *output = (char *)malloc(output_len);
    if (output == NULL) {
        return NULL;
    }

    regex_t regex;
    regmatch_t match[1];
    const char *pattern = "[A-Z]";
    
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        free(output);
        return NULL;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;
    int first_char = 1;

    while (in_idx < input_len) {
        if (regexec(&regex, input + in_idx, 1, match, 0) == 0) {
            size_t match_pos = in_idx + match[0].rm_so;
            
            while (in_idx < match_pos) {
                output[out_idx++] = tolower((unsigned char)input[in_idx++]);
            }
            
            if (!first_char && in_idx > 0) {
                output[out_idx++] = '_';
            }
            
            output[out_idx++] = tolower((unsigned char)input[in_idx++]);
            first_char = 0;
        } else {
            output[out_idx++] = tolower((unsigned char)input[in_idx++]);
        }
    }

    output[out_idx] = '\0';
    regfree(&regex);

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
        char *result = camel_to_snake(test_cases[i]);
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