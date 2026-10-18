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

    char *result = (char *)malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';

    for (i = 0; i < size; i++) {
        if (list[i] != NULL) {
            strcat(result, list[i]);
        }
    }

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