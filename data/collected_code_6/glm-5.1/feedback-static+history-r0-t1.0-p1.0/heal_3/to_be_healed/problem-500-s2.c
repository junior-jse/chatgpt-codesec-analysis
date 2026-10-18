#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concatenate_list(char **list, size_t size) {
    size_t total_length = 0;
    size_t i;
    for (i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], SIZE_MAX);
            if (len == SIZE_MAX) {
                return NULL;
            }
            total_length += len;
        }
    }

    if (size == 0 || total_length == 0) {
        char *empty_str = malloc(1);
        if (empty_str == NULL) {
            return NULL;
        }
        empty_str[0] = '\0';
        return empty_str;
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t current_pos = 0;
    for (i = 0; i < size; i++) {
        if (list[i] != NULL) {
            /* Possible weaknesses found:
             *  each undeclared identifier is reported only once for each function it appears in
             *  use of undeclared identifier 'current_length'
             *  'current_length' undeclared (first use in this function)
             */
            size_t len = strnlen(list[i], SIZE_MAX - current_length);
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
    char *list[] = {"Hello", ", ", "world", "!"};
    size_t size = sizeof(list) / sizeof(list[0]);

    char *concatenated = concatenate_list(list, size);
    if (concatenated == NULL) {
        return EXIT_FAILURE;
    }

    printf("%s\n", concatenated);
    free(concatenated);
    concatenated = NULL;

    return EXIT_SUCCESS;
}