#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** convert_tuple_to_list(char** tuple, int tuple_size, const char* str, int* out_size) {
    if (tuple == NULL || str == NULL || out_size == NULL || tuple_size < 0) {
        return NULL;
    }

    int new_size = tuple_size * 2;
    char** list = (char**)malloc(new_size * sizeof(char*));
    if (list == NULL) {
        return NULL;
    }

    size_t str_len = strlen(str);

    for (int i = 0; i < tuple_size; i++) {
        size_t tuple_elem_len = (tuple[i] != NULL) ? strlen(tuple[i]) : 0;

        list[i * 2] = (char*)malloc(tuple_elem_len + 1);
        if (list[i * 2] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        if (tuple[i] != NULL) {
            strncpy(list[i * 2], tuple[i], tuple_elem_len);
        }
        list[i * 2][tuple_elem_len] = '\0';

        list[i * 2 + 1] = (char*)malloc(str_len + 1);
        if (list[i * 2 + 1] == NULL) {
            free(list[i * 2]);
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        strncpy(list[i * 2 + 1], str, str_len);
        list[i * 2 + 1][str_len] = '\0';
    }

    *out_size = new_size;
    return list;
}

void free_list(char** list, int size) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

int main() {
    char* tuple[] = {"apple", "banana", "cherry"};
    int tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    const char* str_to_add = "fruit";
    int out_size = 0;

    char** list = convert_tuple_to_list(tuple, tuple_size, str_to_add, &out_size);
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < out_size; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list, out_size);

    return EXIT_SUCCESS;
}