#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 65536

char *concatenate_strings(const char **strings, size_t count) {
    if (strings == NULL || count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(size_t)) {
        return NULL;
    }

    size_t *lengths = malloc(count * sizeof(*lengths));
    if (lengths == NULL) {
        return NULL;
    }

    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            free(lengths);
            return NULL;
        }
        const char *terminator = memchr(strings[i], '\0', MAX_STRING_LENGTH);
        if (terminator == NULL) {
            free(lengths);
            return NULL;
        }
        size_t length = (size_t)(terminator - strings[i]);
        if (length > SIZE_MAX - 1 - total_length) {
            free(lengths);
            return NULL;
        }
        lengths[i] = length;
        total_length += length;
    }

    size_t capacity = total_length + 1;
    char *result = malloc(capacity);
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        size_t remaining = capacity - offset;
        if (remaining == 0 || lengths[i] > remaining - 1) {
            free(result);
            free(lengths);
            return NULL;
        }
        memcpy(result + offset, strings[i], lengths[i]);
        offset += lengths[i];
    }
    result[offset] = '\0';

    free(lengths);
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