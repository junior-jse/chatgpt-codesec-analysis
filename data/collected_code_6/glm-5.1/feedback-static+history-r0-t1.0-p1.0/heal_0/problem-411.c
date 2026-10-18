#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *snake_to_camel(const char *snake_str) {
    if (!snake_str) {
        return NULL;
    }

    size_t len = strlen(snake_str);
    if (len == 0) {
        char *empty = malloc(1);
        if (!empty) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    char *result = malloc(len + 1);
    if (!result) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "_[a-z]", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return NULL;
    }

    regmatch_t match;
    size_t src_idx = 0;
    size_t dst_idx = 0;

    while (src_idx < len) {
        if (regexec(&regex, snake_str + src_idx, 1, &match, 0) == 0 && match.rm_so == 0) {
            result[dst_idx++] = snake_str[src_idx + 1] - 32;
            src_idx += match.rm_eo;
        } else {
            result[dst_idx++] = snake_str[src_idx++];
        }
    }

    result[dst_idx] = '\0';
    regfree(&regex);

    return result;
}

int main(void) {
    const char *input = "this_is_a_snake_case_string";
    char *output = snake_to_camel(input);
    
    if (output) {
        printf("%s\n", output);
        free(output);
    } else {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }
    
    return 0;
}