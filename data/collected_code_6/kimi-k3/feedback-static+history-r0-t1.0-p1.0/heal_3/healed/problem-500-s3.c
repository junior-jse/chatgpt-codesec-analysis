#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concatenate_list(const char **list, size_t count) {
    size_t total_length = 0;
    size_t i;
    char *result;
    size_t offset = 0;

    if (list == NULL || count == 0) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        if (list[i] == NULL) {
            return NULL;
        }
        size_t len = strnlen(list[i], 1024);
        if (len == 1024 && list[i][len - 1] != '\0') {
            return NULL;
        }
        if (total_length > SIZE_MAX - len - 1) {
            return NULL;
        }
        total_length += len;
    }

    result = (char*)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        size_t len = strnlen(list[i], 1024);
        if (offset + len > total_length) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, list[i], len);
        offset += len;
    }
    result[total_length] = '\0';

    return result;
}

int main(void) {
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