#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concatenate_list(char **list, size_t size) {
    size_t total_length = 1;
    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], SIZE_MAX);
            if (len == 0 && total_length == 1 && list[i][0] != '\0') {
                return NULL;
            }
            total_length += len;
        }
    }

    char *result = (char *)malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t current_pos = 0;

    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], total_length - current_pos);
            if (current_pos + len >= total_length) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + current_pos, list[i], len);
            current_pos += len;
        }
    }

    result[current_pos] = '\0';
    return result;
}

int main(void) {
    char *list[] = {
        "Hello, ",
        "world",
        "! ",
        NULL,
        "This is a test."
    };

    size_t list_size = sizeof(list) / sizeof(list[0]);

    char *concatenated = concatenate_list(list, list_size);

    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    return 0;
}