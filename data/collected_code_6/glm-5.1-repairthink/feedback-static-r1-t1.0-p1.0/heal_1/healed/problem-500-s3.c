#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concatenate_list(char **list, size_t size) {
    size_t total_length = 1;
    size_t i;
    for (i = 0; i < size; i++) {
        if (list[i] != NULL) {
            total_length += strlen(list[i]);
        }
    }

    char *result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    char *current = result;
    for (i = 0; i < size; i++) {
        if (list[i] != NULL) {
            size_t len = strlen(list[i]);
            memcpy(current, list[i], len);
            current += len;
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