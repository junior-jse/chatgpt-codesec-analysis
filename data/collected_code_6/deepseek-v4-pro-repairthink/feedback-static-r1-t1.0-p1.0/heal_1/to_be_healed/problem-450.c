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

        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
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
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        input[i] = (char *)malloc((strlen(strings[i]) + 1) * sizeof(char));
        if (input[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(input[j]);
            }
            free(input);
            return 1;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
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