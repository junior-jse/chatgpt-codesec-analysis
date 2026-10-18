#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_SAFE_STRLEN 4096

char **tuple_to_list(const char *tuple[], int tuple_size, const char *str, int *list_size) {
    if (list_size == NULL) return NULL;
    if (tuple == NULL || str == NULL || tuple_size < 0) {
        *list_size = 0;
        return NULL;
    }

    size_t str_len = strnlen(str, MAX_SAFE_STRLEN);
    if (str_len == MAX_SAFE_STRLEN) {
        *list_size = 0;
        return NULL;
    }

    if ((size_t)tuple_size > SIZE_MAX / (2 * sizeof(char *))) {
        *list_size = 0;
        return NULL;
    }
    size_t total = (size_t)tuple_size * 2;
    char **list = malloc(total * sizeof(char *));
    if (list == NULL) {
        *list_size = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < tuple_size; i++) {
        size_t elem_len;
        const char *src = tuple[i] ? tuple[i] : "";
        if (tuple[i] != NULL) {
            elem_len = strnlen(tuple[i], MAX_SAFE_STRLEN);
            if (elem_len == MAX_SAFE_STRLEN) {
                for (int j = 0; j < index; j++) free(list[j]);
                free(list);
                *list_size = 0;
                return NULL;
            }
        } else {
            elem_len = 0;
        }

        if (elem_len == SIZE_MAX) {
            for (int j = 0; j < index; j++) free(list[j]);
            free(list);
            *list_size = 0;
            return NULL;
        }

        list[index] = malloc(elem_len + 1);
        if (list[index] == NULL) {
            for (int j = 0; j < index; j++) free(list[j]);
            free(list);
            *list_size = 0;
            return NULL;
        }
        snprintf(list[index], elem_len + 1, "%.*s", (int)elem_len, src);
        index++;

        if (str_len == SIZE_MAX) {
            for (int j = 0; j < index; j++) free(list[j]);
            free(list);
            *list_size = 0;
            return NULL;
        }
        list[index] = malloc(str_len + 1);
        if (list[index] == NULL) {
            for (int j = 0; j < index; j++) free(list[j]);
            free(list);
            *list_size = 0;
            return NULL;
        }
        snprintf(list[index], str_len + 1, "%.*s", (int)str_len, str);
        index++;
    }

    *list_size = index;
    return list;
}

int main(void) {
    const char *tuple[] = {"a", "b", "c"};
    int tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    int list_size = 0;

    char **list = tuple_to_list(tuple, tuple_size, "-", &list_size);
    if (list == NULL) return 1;

    for (int i = 0; i < list_size; i++) {
        printf("%s\n", list[i]);
        free(list[i]);
    }
    free(list);

    return 0;
}