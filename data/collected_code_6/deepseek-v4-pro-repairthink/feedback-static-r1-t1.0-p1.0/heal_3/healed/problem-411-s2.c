#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 4096

static size_t safe_strlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

char *snake_to_camel(const char *snake_str) {
    if (snake_str == NULL) {
        return NULL;
    }

    size_t len = safe_strlen(snake_str, MAX_INPUT_LEN);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t dst = 0;
    int capitalize_next = 0;

    for (size_t i = 0; i < len; i++) {
        char c = snake_str[i];
        if (c == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next) {
                if (c >= 'a' && c <= 'z') {
                    c -= ('a' - 'A');
                }
                capitalize_next = 0;
            }
            result[dst++] = c;
        }
    }

    result[dst] = '\0';
    return result;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <snake_case_string>\n", argv[0]);
        return 1;
    }

    char *camel_str = snake_to_camel(argv[1]);
    if (camel_str == NULL) {
        fprintf(stderr, "Error converting string\n");
        return 1;
    }

    printf("%s\n", camel_str);
    free(camel_str);
    return 0;
}