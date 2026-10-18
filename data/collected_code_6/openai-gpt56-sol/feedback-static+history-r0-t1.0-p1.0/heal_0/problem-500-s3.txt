#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *concatenate(const char *const elements[], size_t count)
{
    size_t total_length = 0;

    if (elements == NULL && count != 0) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t length;

        if (elements[i] == NULL) {
            return NULL;
        }

        length = strlen(elements[i]);
        if (length > SIZE_MAX - total_length - 1) {
            return NULL;
        }

        total_length += length;
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    char *destination = result;
    for (size_t i = 0; i < count; ++i) {
        size_t length = strlen(elements[i]);
        memcpy(destination, elements[i], length);
        destination += length;
    }

    *destination = '\0';
    return result;
}

int main(void)
{
    const char *elements[] = {"Hello", ", ", "world", "!"};
    size_t count = sizeof(elements) / sizeof(elements[0]);
    char *result = concatenate(elements, count);

    if (result == NULL) {
        fputs("Failed to concatenate elements.\n", stderr);
        return EXIT_FAILURE;
    }

    if (puts(result) == EOF) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}