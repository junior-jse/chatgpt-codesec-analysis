#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *snake_to_camel(const char *input) {
    if (!input) {
        return NULL;
    }

    size_t len = strlen(input);
    char *result = (char *)malloc(len + 1);
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
        size_t match_start = matches[0].rm_so;
        size_t match_end = matches[0].rm_eo;
        size_t group_start = matches[1].rm_so;
        size_t group_end = matches[1].rm_eo;

        if (match_start > 0) {
            memcpy(result + dst_idx, cursor, match_start);
            dst_idx += match_start;
        }

        if (group_start >= 0 && group_end > group_start) {
            result[dst_idx++] = cursor[group_start] - 32;
        }

        src_idx += match_end;
        cursor += match_end;
    }

    if (src_idx < len) {
        size_t remaining = len - src_idx;
        memcpy(result + dst_idx, cursor, remaining);
        dst_idx += remaining;
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