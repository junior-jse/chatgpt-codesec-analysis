#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *snake_to_camel(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t len = strlen(input);
    size_t max_result_len = len + 1;
    char *result = malloc(max_result_len);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t input_index = 0;
    int capitalize_next = 0;

    while (input_index < len) {
        char current = input[input_index];

        if (current == '_') {
            capitalize_next = 1;
            input_index++;
            continue;
        }

        if (capitalize_next) {
            if (result_index >= max_result_len - 1) {
                free(result);
                return NULL;
            }
            result[result_index++] = (char)toupper((unsigned char)current);
            capitalize_next = 0;
        } else {
            if (result_index >= max_result_len - 1) {
                free(result);
                return NULL;
            }
            result[result_index++] = current;
        }

        input_index++;
    }

    result[result_index] = '\0';
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "hello_world",
        "snake_case_string",
        "alreadyCamel",
        "multiple__underscores",
        "_leading_underscore",
        "trailing_underscore_",
        "",
        "no_underscores_here",
        "a_b_c_d_e"
    };

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < num_tests; i++) {
        char *converted = snake_to_camel(test_cases[i]);
        if (converted != NULL) {
            printf("\"%s\" -> \"%s\"\n", test_cases[i], converted);
            free(converted);
        } else {
            printf("\"%s\" -> NULL\n", test_cases[i]);
        }
    }

    return 0;
}