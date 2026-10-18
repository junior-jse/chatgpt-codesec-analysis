#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *camel_to_snake(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    regmatch_t matches[3];
    const char *pattern = "([a-z0-9])([A-Z])";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t input_len = strlen(input);
    size_t out_len = input_len + 1;
    size_t pos = 0;

    while (regexec(&regex, input + pos, 3, matches, 0) == 0) {
        out_len += 1;
        pos += matches[0].rm_eo;
    }

    if (out_len == input_len + 1) {
        regfree(&regex);
        char *copy = strdup(input);
        return copy;
    }

    char *result = (char *)malloc(out_len);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t src = 0;
    size_t dst = 0;

    while (regexec(&regex, input + src, 3, matches, 0) == 0) {
        size_t match_start = matches[0].rm_so;
        size_t match_end = matches[0].rm_eo;
        size_t g2_start = matches[2].rm_so;

        memcpy(result + dst, input + src, match_start);
        dst += match_start;

        result[dst++] = input[src + match_start];
        result[dst++] = '_';
        result[dst++] = input[src + g2_start] | 0x20;

        src += match_end;
    }

    size_t remaining = input_len - src;
    if (remaining > 0) {
        memcpy(result + dst, input + src, remaining);
        dst += remaining;
    }

    result[dst] = '\0';
    regfree(&regex);

    return result;
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