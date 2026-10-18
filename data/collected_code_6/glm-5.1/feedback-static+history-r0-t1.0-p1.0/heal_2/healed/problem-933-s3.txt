#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *camel_to_snake(const char *camel_str) {
    if (camel_str == NULL) {
        return NULL;
    }

    size_t len = strlen(camel_str);
    size_t max_len = len * 2 + 1;
    char *result = malloc(max_len);
    if (result == NULL) {
        return NULL;
    }

    size_t r_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && isupper((unsigned char)camel_str[i]) && islower((unsigned char)camel_str[i - 1])) {
            if (r_idx < max_len - 1) {
                result[r_idx++] = '_';
            } else {
                break;
            }
        }
        if (r_idx < max_len - 1) {
            result[r_idx++] = tolower((unsigned char)camel_str[i]);
        } else {
            break;
        }
    }
    result[r_idx] = '\0';

    char *final_str = strdup(result);
    free(result);
    return final_str;
}

int main(int argc, char * const argv[]) {
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