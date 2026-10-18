#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** extract_substrings(char** strings, int count, int size) {
    if (strings == NULL || count <= 0 || size < 0) {
        return NULL;
    }

    char** result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            result[i] = NULL;
            continue;
        }

        result[i] = malloc(size + 1);
        if (result[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        size_t len = strnlen(strings[i], (size_t)size);
        size_t copy_size = len < (size_t)size ? len : (size_t)size;
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], strings[i], copy_size);
        result[i][copy_size] = '\0';
    }

    result[count] = NULL;

    return result;
}

void free_extracted_strings(char** strings) {
    if (strings == NULL) {
        return;
    }

    for (int i = 0; strings[i] != NULL; i++) {
        free(strings[i]);
    }
    free(strings);
}

int main() {
    char* input_strings[] = {
        "Hello, World!",
        "Programming in C",
        "Extract substrings",
        NULL
    };

    int count = 0;
    while (input_strings[count] != NULL) {
        count++;
    }

    int extract_size = 5;

    char** extracted = extract_substrings(input_strings, count, extract_size);
    if (extracted == NULL) {
        return EXIT_FAILURE;
    }

    for (int i = 0; extracted[i] != NULL; i++) {
        printf("%s\n", extracted[i]);
    }

    free_extracted_strings(extracted);

    return EXIT_SUCCESS;
}