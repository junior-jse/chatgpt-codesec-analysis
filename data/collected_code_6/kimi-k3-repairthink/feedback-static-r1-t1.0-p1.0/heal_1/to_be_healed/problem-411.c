#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LEN 1024

char *snake_to_camel(const char *input) {
    regex_t regex;
    regmatch_t match;
    char *result = NULL;
    size_t result_len = 0;
    size_t result_capacity = 0;
    const char *cursor = input;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    result_capacity = strlen(input) + 1;
    result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    ret = regcomp(&regex, "_([a-z])", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return NULL;
    }

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == 0) {
            size_t prefix_len = (size_t)match.rm_so;
            if (result_len + prefix_len + 2 > result_capacity) {
                result_capacity = (result_len + prefix_len + 2) * 2;
                char *new_result = realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = new_result;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, cursor, prefix_len);
            result_len += prefix_len;
            result[result_len++] = (char)(cursor[match.rm_eo - 1] - 32);
            result[result_len] = '\0';
            cursor += match.rm_eo;
        } else if (ret == REG_NOMATCH) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(cursor);
            if (result_len + remaining + 1 > result_capacity) {
                result_capacity = result_len + remaining + 1;
                char *new_result = realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = new_result;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, cursor, remaining);
            result_len += remaining;
            result[result_len] = '\0';
            break;
        } else {
            free(result);
            regfree(&regex);
            return NULL;
        }
    }

    regfree(&regex);
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "hello_world",
        "snake_case_string_example",
        "single",
        "multiple__underscores",
        NULL
    };
    int i;

    for (i = 0; test_cases[i] != NULL; i++) {
        char *camel = snake_to_camel(test_cases[i]);
        if (camel != NULL) {
            printf("Input:  %s\n", test_cases[i]);
            printf("Output: %s\n\n", camel);
            free(camel);
        } else {
            printf("Conversion failed for: %s\n\n", test_cases[i]);
        }
    }

    return 0;
}