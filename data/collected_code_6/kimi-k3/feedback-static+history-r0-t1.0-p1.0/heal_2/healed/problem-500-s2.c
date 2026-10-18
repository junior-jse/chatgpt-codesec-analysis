#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concatenate_strings(const char **strings, size_t count) {
    if (strings == NULL || count == 0) {
        return NULL;
    }

    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            return NULL;
        }
        size_t len = 0;
        while (strings[i][len] != '\0') {
            len++;
        }
        total_length += len;
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = 0;
        while (strings[i][len] != '\0') {
            len++;
        }
        if (offset + len + 1 > total_length + 1) {
            free(result);
            return NULL;
        }
        for (size_t j = 0; j < len; j++) {
            result[offset + j] = strings[i][j];
        }
        offset += len;
    }
    result[offset] = '\0';

    return result;
}

int main(void) {
    const char *strings[] = {"Hello", " ", "World", "!"};
    size_t count = sizeof(strings) / sizeof(strings[0]);

    char *concatenated = concatenate_strings(strings, count);
    if (concatenated == NULL) {
        fprintf(stderr, "Failed to concatenate strings\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", concatenated);
    free(concatenated);

    return EXIT_SUCCESS;
}