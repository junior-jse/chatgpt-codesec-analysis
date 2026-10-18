#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *snake_to_camel(const char *snake_str) {
    if (snake_str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(snake_str);
    if (len == 0) {
        char *empty = malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    char *camel_str = malloc(len + 1);
    if (camel_str == NULL) {
        return NULL;
    }

    size_t camel_idx = 0;
    int capitalize_next = 0;

    for (size_t i = 0; i < len; ++i) {
        if (snake_str[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                camel_str[camel_idx++] = toupper((unsigned char)snake_str[i]);
                capitalize_next = 0;
            } else {
                camel_str[camel_idx++] = snake_str[i];
            }
        }
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