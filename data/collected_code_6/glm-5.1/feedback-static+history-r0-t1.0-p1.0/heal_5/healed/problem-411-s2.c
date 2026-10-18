#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *snake_to_camel(const char *snake_str) {
    if (snake_str == NULL) {
        return NULL;
    }

    size_t len = strlen(snake_str);
    if (len == 0) {
        char *empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    size_t max_len = len + 1;
    char *camel_str = malloc(max_len);
    if (camel_str == NULL) {
        return NULL;
    }

    size_t camel_idx = 0;
    const char *cursor = snake_str;
    const char *str_end = snake_str + len;

    while (*cursor) {
        if (*cursor == '_' && (cursor + 1) < str_end) {
            cursor++;
            if (*cursor >= 'a' && *cursor <= 'z') {
                if (camel_idx >= max_len) {
                    free(camel_str);
                    return NULL;
                }
                camel_str[camel_idx++] = *cursor - ('a' - 'A');
            } else if (*cursor == '_') {
                if (camel_idx >= max_len) {
                    free(camel_str);
                    return NULL;
                }
                camel_str[camel_idx++] = '_';
            } else {
                if (camel_idx >= max_len) {
                    free(camel_str);
                    return NULL;
                }
                camel_str[camel_idx++] = *cursor;
            }
        } else {
            if (camel_idx >= max_len) {
                free(camel_str);
                return NULL;
            }
            camel_str[camel_idx++] = *cursor;
        }
        cursor++;
    }

    if (camel_idx >= max_len) {
        free(camel_str);
        return NULL;
    }
    camel_str[camel_idx] = '\0';

    char *shrunk = realloc(camel_str, camel_idx + 1);
    if (shrunk != NULL) {
        camel_str = shrunk;
    }

    return camel_str;
}

int main(void) {
    const char *input = "this_is_a_snake_case_string_123";
    char *output = snake_to_camel(input);
    if (output == NULL) {
        return 1;
    }
    printf("%s\n", output);
    free(output);
    return 0;
}