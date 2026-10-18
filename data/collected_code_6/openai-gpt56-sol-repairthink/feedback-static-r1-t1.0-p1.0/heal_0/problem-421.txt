#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *join_tuple(const char *const elements[], size_t count, const char *delimiter)
{
    size_t delimiter_length;
    size_t total_length = 1;
    char *result;
    char *position;

    if (delimiter == NULL || (count > 0 && elements == NULL)) {
        return NULL;
    }

    delimiter_length = strlen(delimiter);

    for (size_t i = 0; i < count; ++i) {
        size_t element_length;

        if (elements[i] == NULL) {
            return NULL;
        }

        element_length = strlen(elements[i]);

        if (element_length > SIZE_MAX - total_length) {
            return NULL;
        }
        total_length += element_length;

        if (i + 1 < count) {
            if (delimiter_length > SIZE_MAX - total_length) {
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    position = result;

    for (size_t i = 0; i < count; ++i) {
        size_t element_length = strlen(elements[i]);

        memcpy(position, elements[i], element_length);
        position += element_length;

        if (i + 1 < count) {
            memcpy(position, delimiter, delimiter_length);
            position += delimiter_length;
        }
    }

    *position = '\0';
    return result;
}

int main(void)
{
    const char *tuple[] = {"apple", "banana", "cherry"};
    const size_t count = sizeof tuple / sizeof tuple[0];
    char *joined = join_tuple(tuple, count, ", ");

    if (joined == NULL) {
        fputs("Failed to concatenate tuple elements.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", joined) < 0) {
        free(joined);
        return EXIT_FAILURE;
    }

    free(joined);
    return EXIT_SUCCESS;
}