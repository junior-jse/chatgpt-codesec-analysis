#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_to_list(const char *str, size_t buf_size, size_t *list_len) {
    if (str == NULL || list_len == NULL || buf_size == 0) {
        return NULL;
    }

    size_t len = strnlen(str, buf_size);
    if (len == 0) {
        *list_len = 0;
        return NULL;
    }

    char **list = malloc((len + 1) * sizeof(char *));
    if (list == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        list[i] = malloc(2 * sizeof(char));
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        list[i][0] = str[i];
        list[i][1] = '\0';
    }

    list[len] = NULL;
    *list_len = len;
    return list;
}

void free_list(char **list, size_t list_len) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list_len; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *str = "hello";
    size_t list_len = 0;
    size_t buf_size = sizeof("hello");

    char **list = string_to_list(str, buf_size, &list_len);
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list_len; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list, list_len);

    return EXIT_SUCCESS;
}