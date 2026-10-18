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
        total_length += strlen(strings[i]);
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    for (size_t i = 0; i < count; i++) {
        strcat(result, strings[i]);
    }

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