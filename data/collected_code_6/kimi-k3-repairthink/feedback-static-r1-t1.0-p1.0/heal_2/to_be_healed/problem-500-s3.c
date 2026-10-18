#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

char *concatenate_list(const char **list, size_t count)
{
    size_t total_length = 0;
    size_t i;
    char *result;
    size_t offset = 0;

    if (list == NULL || count == 0) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        size_t len;
        if (list[i] == NULL) {
            return NULL;
        }
        len = strnlen(list[i], MAX_STRING_LENGTH + 1);
        if (len > MAX_STRING_LENGTH) {
            return NULL;
        }
        if (total_length + len < total_length) {
            return NULL;
        }
        total_length += len;
    }

    result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        size_t len = strnlen(list[i], MAX_STRING_LENGTH + 1);
        if (len > MAX_STRING_LENGTH || len > total_length - offset) {
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, list[i], len);
        offset += len;
    }
    result[offset] = '\0';

    return result;
}

int main(void)
{
    const char *list[] = {"Hello", " ", "World", "!"};
    size_t count = sizeof(list) / sizeof(list[0]);
    char *concatenated;

    concatenated = concatenate_list(list, count);
    if (concatenated == NULL) {
        fprintf(stderr, "Error: Failed to concatenate strings\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", concatenated);

    free(concatenated);

    return EXIT_SUCCESS;
}