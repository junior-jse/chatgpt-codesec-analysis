#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(char **input, size_t count, size_t size) {
    if (input == NULL || count == 0 || size == 0) {
        return NULL;
    }

    char **result = (char **)malloc(count * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        if (input[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[i] = (char *)malloc((size + 1) * sizeof(char));
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[i], input[i], size);
        result[i][size] = '\0';
    }

    return result;
}

int main(void) {
    const char *strings[] = {"Hello", "World", "C", "Programming"};
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t extract_size = 3;

    char **input = (char **)malloc(count * sizeof(char *));
    if (input == NULL) {
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        input[i] = (char *)malloc((strlen(strings[i]) + 1) * sizeof(char));
        if (input[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(input[j]);
            }
            free(input);
            return 1;
        }
        strcpy(input[i], strings[i]);
    }

    char **extracted = extract_strings(input, count, extract_size);
    if (extracted == NULL) {
        for (size_t i = 0; i < count; i++) {
            free(input[i]);
        }
        free(input);
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", extracted[i]);
        free(extracted[i]);
        free(input[i]);
    }

    free(extracted);
    free(input);
    return 0;
}