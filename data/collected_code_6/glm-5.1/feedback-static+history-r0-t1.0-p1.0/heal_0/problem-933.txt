#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

char *camel_to_snake(const char *camel_str) {
    if (camel_str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "([A-Z])", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t len = strlen(camel_str);
    size_t max_snake_len = len * 2 + 1;
    char *snake_str = (char *)malloc(max_snake_len);
    if (snake_str == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t out_idx = 0;
    const char *cursor = camel_str;
    regmatch_t matches[2];

    while (1) {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            free(snake_str);
            regfree(&regex);
            return NULL;
        }

        size_t prefix_len = matches[1].rm_so;
        if (out_idx + prefix_len + 2 >= max_snake_len) {
            free(snake_str);
            regfree(&regex);
            return NULL;
        }

        for (size_t i = 0; i < prefix_len; i++) {
            snake_str[out_idx++] = cursor[i];
        }

        snake_str[out_idx++] = '_';
        snake_str[out_idx++] = tolower((unsigned char)cursor[matches[1].rm_so]);

        cursor += matches[1].rm_eo;
    }

    size_t remaining_len = strlen(cursor);
    if (out_idx + remaining_len + 1 >= max_snake_len) {
        free(snake_str);
        regfree(&regex);
        return NULL;
    }

    for (size_t i = 0; i < remaining_len; i++) {
        snake_str[out_idx++] = cursor[i];
    }

    snake_str[out_idx] = '\0';
    regfree(&regex);

    char *shrunk = (char *)realloc(snake_str, out_idx + 1);
    if (shrunk != NULL) {
        snake_str = shrunk;
    }

    return snake_str;
}

int main(void) {
    const char *inputs[] = {
        "camelCaseString",
        "AnotherTestCase",
        "simple",
        "A",
        "XMLParser",
        ""
    };

    size_t count = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < count; i++) {
        char *result = camel_to_snake(inputs[i]);
        if (result != NULL) {
            printf("\"%s\" -> \"%s\"\n", inputs[i], result);
            free(result);
        } else {
            printf("Error processing \"%s\"\n", inputs[i]);
        }
    }

    return 0;
}