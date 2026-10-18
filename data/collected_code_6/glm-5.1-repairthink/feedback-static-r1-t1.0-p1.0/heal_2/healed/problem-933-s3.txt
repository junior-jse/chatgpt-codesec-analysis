#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *camel_to_snake(const char *camel_str) {
    if (camel_str == NULL) {
        return NULL;
    }

    size_t in_len = strlen(camel_str);
    if (in_len > SIZE_MAX / 2) {
        return NULL;
    }

    size_t max_len = in_len * 2 + 1;
    char *result = malloc(max_len);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < in_len; i++) {
        unsigned char c = camel_str[i];
        if (c >= 'A' && c <= 'Z') {
            if (i > 0) {
                unsigned char prev = camel_str[i - 1];
                if ((prev >= 'a' && prev <= 'z') || (prev >= '0' && prev <= '9')) {
                    result[j++] = '_';
                }
            }
            result[j++] = c + 32;
        } else {
            result[j++] = c;
        }
    }
    result[j] = '\0';

    char *final_str = realloc(result, j + 1);
    if (final_str != NULL) {
        result = final_str;
    }

    return result;
}

int main(const int argc, const char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camel_case_string>\n", argv[0]);
        return 1;
    }

    char *snake_str = camel_to_snake(argv[1]);
    if (snake_str == NULL) {
        fprintf(stderr, "Conversion failed\n");
        return 1;
    }

    printf("%s\n", snake_str);

    free(snake_str);
    return 0;
}