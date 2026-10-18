#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char *remove_non_alphanumeric(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, SIZE_MAX);
    if (input_len == SIZE_MAX) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[^a-zA-Z0-9]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    char *result = (char *)malloc(input_len + 1);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t result_index = 0;
    regmatch_t match;
    const char *cursor = input;
    size_t remaining_input = input_len;

    while (remaining_input > 0 && regexec(&regex, cursor, 1, &match, 0) == 0) {
        size_t match_start = match.rm_so;
        size_t match_end = match.rm_eo;

        if (match_start > remaining_input || match_end > remaining_input || match_start > match_end) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (match_start > 0) {
            if (result_index + match_start > input_len) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            memcpy(result + result_index, cursor, match_start);
            result_index += match_start;
        }

        cursor += match_end;
        remaining_input -= match_end;
    }

    size_t remaining_len = strnlen(cursor, remaining_input);
    if (remaining_len > 0) {
        if (result_index + remaining_len > input_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        memcpy(result + result_index, cursor, remaining_len);
        result_index += remaining_len;
    }

    result[result_index] = '\0';
    regfree(&regex);
    return result;
}

int main(void) {
    const char *test_string = "Hello, World! 123...";
    char *cleaned = remove_non_alphanumeric(test_string);
    
    if (cleaned != NULL) {
        printf("%s\n", cleaned);
        free(cleaned);
    }

    return 0;
}