#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

static char *snake_to_camel(const char *input) {
    if (!input) {
        return NULL;
    }

    size_t len = strnlen(input, SIZE_MAX - 1);
    if (len >= SIZE_MAX - 1) {
        return NULL;
    }

    size_t result_capacity = len + 1;
    char *result = (char *)malloc(result_capacity);
    if (!result) {
        return NULL;
    }

    regex_t regex;
    regmatch_t matches[2];
    const char *pattern = "_([a-z])";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        free(result);
        return NULL;
    }

    size_t src_idx = 0;
    size_t dst_idx = 0;
    const char *cursor = input;

    while (src_idx < len && regexec(&regex, cursor, 2, matches, 0) == 0) {
        size_t match_start = (size_t)matches[0].rm_so;
        size_t match_end = (size_t)matches[0].rm_eo;
        size_t group_start = (size_t)matches[1].rm_so;
        size_t group_end = (size_t)matches[1].rm_eo;

        if (match_start > 0) {
            if (dst_idx + match_start >= result_capacity) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + dst_idx, cursor, match_start);
            dst_idx += match_start;
        }

        if (group_end > group_start) {
            if (dst_idx >= result_capacity - 1) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            result[dst_idx++] = (char)(cursor[group_start] - 32);
        }

        if (src_idx + match_end > len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        src_idx += match_end;
        cursor += match_end;
    }

    if (src_idx < len) {
        size_t remaining = len - src_idx;
        if (dst_idx + remaining >= result_capacity) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + dst_idx, cursor, remaining);
        dst_idx += remaining;
    }

    if (dst_idx >= result_capacity) {
        free(result);
        regfree(&regex);
        return NULL;
    }
    result[dst_idx] = '\0';
    regfree(&regex);

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