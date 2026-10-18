#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    size_t input_len = strnlen(input, 4096);
    if (input_len == 0 || input_len >= 4096) {
        if (input_len == 0) {
            char *empty = (char *)malloc(1);
            if (empty != NULL) {
                empty[0] = '\0';
            }
            return empty;
        }
        return NULL;
    }

    regex_t regex;
    const char *pattern = "([a-z0-9])([A-Z])";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t max_output_len = input_len * 2 + 1;
    char *result = (char *)malloc(max_output_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t src_pos = 0;
    size_t dst_pos = 0;
    int found = 0;
    regmatch_t matches[3];

    while (src_pos < input_len && regexec(&regex, input + src_pos, 3, matches, 0) == 0) {
        size_t match_start = (size_t)matches[0].rm_so;
        size_t match_end = (size_t)matches[0].rm_eo;
        size_t first_end = (size_t)matches[1].rm_eo;
        size_t second_start = (size_t)matches[2].rm_so;

        size_t prefix_len = match_start;
        size_t first_char_offset = first_end - 1;

        size_t needed = dst_pos + prefix_len + 3;
        if (needed > max_output_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (prefix_len > 0) {
            memcpy(result + dst_pos, input + src_pos, prefix_len);
            dst_pos += prefix_len;
        }

        result[dst_pos++] = input[src_pos + first_char_offset];
        result[dst_pos++] = '_';
        result[dst_pos++] = (char)tolower((unsigned char)input[src_pos + second_start]);

        src_pos += match_end;
        found = 1;
    }

    if (src_pos < input_len) {
        size_t remaining = input_len - src_pos;
        if (dst_pos + remaining + 1 > max_output_len) {
            free(result);
            regfree(&regex);
            return NULL;
        }
        memcpy(result + dst_pos, input + src_pos, remaining);
        dst_pos += remaining;
    }

    result[dst_pos] = '\0';
    regfree(&regex);

    char *final_result = NULL;
    if (!found) {
        final_result = (char *)malloc(input_len + 1);
        if (final_result != NULL) {
            memcpy(final_result, input, input_len + 1);
        }
        free(result);
    } else {
        final_result = (char *)realloc(result, dst_pos + 1);
        if (final_result == NULL) {
            free(result);
        }
    }

    return final_result;
}

int main(void) {
    const char *test_strings[] = {
        "camelCase",
        "snake_case",
        "myVariableName",
        "HTTPResponseCode",
        "test123Case",
        "already_snake",
        "A",
        "",
        "ABC",
        "convertJSONToXML"
    };

    size_t test_count = sizeof(test_strings) / sizeof(test_strings[0]);

    for (size_t i = 0; i < test_count; i++) {
        char *converted = camel_to_snake(test_strings[i]);
        if (converted != NULL) {
            printf("\"%s\" -> \"%s\"\n", test_strings[i], converted);
            free(converted);
        } else {
            printf("\"%s\" -> conversion failed\n", test_strings[i]);
        }
    }

    return 0;
}