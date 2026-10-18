#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LEN 1024

char *snake_to_camel(const char *input, size_t input_len) {
    regex_t regex;
    regmatch_t match;
    char *result = NULL;
    size_t result_len = 0;
    size_t result_capacity = 0;
    const char *cursor = input;
    size_t remaining_len = input_len;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    result_capacity = input_len + 1;
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

    while (remaining_len > 0) {
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
            if (prefix_len > 0 && prefix_len <= remaining_len) {
                memcpy(result + result_len, cursor, prefix_len);
                result_len += prefix_len;
            }
            if ((size_t)match.rm_eo <= remaining_len) {
                result[result_len++] = (char)(cursor[match.rm_eo - 1] - 32);
            }
            result[result_len] = '\0';
            cursor += match.rm_eo;
            remaining_len -= (size_t)match.rm_eo;
        } else if (ret == REG_NOMATCH) {
            if (result_len + remaining_len + 1 > result_capacity) {
                result_capacity = result_len + remaining_len + 1;
                char *new_result = realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = new_result;
            }
            if (remaining_len > 0) {
                memcpy(result + result_len, cursor, remaining_len);
                result_len += remaining_len;
            }
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
        size_t len = strnlen(test_cases[i], MAX_STRING_LEN);
        char *camel = snake_to_camel(test_cases[i], len);
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