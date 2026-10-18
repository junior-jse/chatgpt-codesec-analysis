#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *concatenate_list(char **list, size_t size) {
    size_t total_length = 1;
    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strlen(list[i]);
            if (len > SIZE_MAX - total_length) {
                return NULL;
            }
            total_length += len;
        }
    }

    char *result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    char *current = result;
    size_t remaining = total_length;

    for (size_t i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strlen(list[i]);
            if (len > remaining - 1) {
                break;
            }
            memcpy(current, list[i], len);
            current += len;
            remaining -= len;
        }
    }
    *current = '\0';

    return result;
}

int main(void) {
    char *list[] = {"Hello", ", ", "world", "!"};
    size_t size = sizeof(list) / sizeof(list[0]);

    char *concatenated = concatenate_list(list, size);
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
    } else {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    return 0;
}