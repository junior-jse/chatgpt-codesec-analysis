#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024

char *snake_to_camel(const char *input) {
    if (!input) {
        return NULL;
    }

    size_t len = 0;
    /* Possible weaknesses found:
     *  Array index 'len' is used before limits check. [arrayIndexThenCheck]
     */
    while (input[len] != '\0' && len < MAX_INPUT_LENGTH) {
        len++;
    }
    if (len == MAX_INPUT_LENGTH && input[len] != '\0') {
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (!result) {
        return NULL;
    }

    size_t src = 0, dst = 0;

    while (src < len) {
        size_t p = src;
        while (p < len && !(input[p] == '_' && p + 1 < len && input[p + 1] >= 'a' && input[p + 1] <= 'z')) {
            p++;
        }
        if (p < len) {
            if (p > src) {
                size_t copy_len = p - src;
                if (dst + copy_len > len) {
                    free(result);
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + dst, input + src, copy_len);
                dst += copy_len;
            }
            if (dst >= len) {
                free(result);
                return NULL;
            }
            result[dst++] = input[p + 1] - 32;
            src = p + 2;
        } else {
            size_t remaining = len - src;
            if (dst + remaining > len) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + dst, input + src, remaining);
            dst += remaining;
            src = len;
        }
    }

    result[dst] = '\0';
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