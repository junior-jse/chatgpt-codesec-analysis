#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *concatenate_list(char **list, size_t size) {
    size_t total_length = 0;
    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strlen(list[i]);
            if (len > SIZE_MAX - total_length) {
                return NULL;
            }
            total_length += len;
        }
    }

    if (size == 0 || total_length == 0) {
        char *empty_str = malloc(1);
        if (empty_str == NULL) {
            return NULL;
        }
        empty_str[0] = '\0';
        return empty_str;
    }

    if (total_length == SIZE_MAX) {
        return NULL;
    }

    size_t alloc_size = total_length + 1;

    char *result = malloc(alloc_size);
    if (result == NULL) {
        return NULL;
    }

    char *ptr = result;
    size_t remaining = total_length;
    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], remaining);
            if (len > 0) {
                memcpy(ptr, list[i], len);
                ptr += len;
                remaining -= len;
            }
        }
    }
    *ptr = '\0';

    return result;
}

int main(void) {
    char *list[] = {"Hello", ", ", "world", "!"};
    size_t size = sizeof(list) / sizeof(list[0]);

    char *concatenated = concatenate_list(list, size);
    if (concatenated == NULL) {
        return EXIT_FAILURE;
    }

    printf("%s\n", concatenated);
    free(concatenated);
    concatenated = NULL;

    return EXIT_SUCCESS;
}