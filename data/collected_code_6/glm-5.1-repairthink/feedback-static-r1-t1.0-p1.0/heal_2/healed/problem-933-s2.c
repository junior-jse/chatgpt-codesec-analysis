#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include <stdint.h>

char *camel_to_snake(const char *camel_str, size_t len) {
    if (camel_str == NULL) {
        return NULL;
    }

    if (len == 0) {
        char *empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    char *safe_str = malloc(len + 1);
    if (safe_str == NULL) {
        return NULL;
    }
    memcpy(safe_str, camel_str, len);
    safe_str[len] = '\0';

    regex_t regex;
    int ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        free(safe_str);
        return NULL;
    }

    if (len > SIZE_MAX / 2) {
        regfree(&regex);
        free(safe_str);
        return NULL;
    }

    char *snake_str = malloc(len * 2 + 1);
    if (snake_str == NULL) {
        regfree(&regex);
        free(safe_str);
        return NULL;
    }

    regmatch_t match;
    size_t src_offset = 0;
    size_t dest_offset = 0;

    while (src_offset < len) {
        int exec_ret = regexec(&regex, safe_str + src_offset, 1, &match, 0);
        
        if (exec_ret == 0 && match.rm_so != -1) {
            size_t match_pos = src_offset + match.rm_so;

            while (src_offset < match_pos) {
                snake_str[dest_offset++] = tolower((unsigned char)safe_str[src_offset++]);
            }

            snake_str[dest_offset++] = '_';
            snake_str[dest_offset++] = tolower((unsigned char)safe_str[src_offset++]);
        } else {
            while (src_offset < len) {
                snake_str[dest_offset++] = tolower((unsigned char)safe_str[src_offset++]);
            }
        }
    }

    snake_str[dest_offset] = '\0';
    regfree(&regex);
    free(safe_str);

    char *final_str = realloc(snake_str, dest_offset + 1);
    if (final_str == NULL) {
        free(snake_str);
        return NULL;
    }

    return final_str;
}

int main(void) {
    const char *inputs[] = {
        "camelCaseString",
        "AnotherExample",
        "single",
        "XMLHttpRequest",
        "A",
        ""
    };
    size_t count = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < count; i++) {
        char *result = camel_to_snake(inputs[i], strnlen(inputs[i], 256));
        if (result == NULL) {
            printf("Input: %s\nOutput: [Allocation or regex error]\n\n", inputs[i]);
        } else {
            printf("Input: %s\nOutput: %s\n\n", inputs[i], result);
            free(result);
        }
    }

    return 0;
}