#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** tuple_to_list(char** tuple, int size, const char* str) {
    if (tuple == NULL || str == NULL || size <= 0) {
        return NULL;
    }

    size_t str_len = strnlen(str, size * 2 > 0 ? (size_t)(size * 2) : 1);
    if (str_len == 0 && str[0] != '\0') {
        return NULL;
    }

    char** list = (char**)calloc((size_t)(size * 2), sizeof(char*));
    if (list == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }

        size_t tuple_len = strnlen(tuple[i], size * 2 > 0 ? (size_t)(size * 2) : 1);
        if (tuple_len == 0 && tuple[i][0] != '\0') {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }

        list[i * 2] = (char*)malloc(tuple_len + 1);
        if (list[i * 2] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }

        if (tuple_len + 1 > tuple_len) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(list[i * 2], tuple[i], tuple_len + 1);
        } else {
            free(list[i * 2]);
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }

        list[i * 2 + 1] = (char*)malloc(str_len + 1);
        if (list[i * 2 + 1] == NULL) {
            for (int j = 0; j <= i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }

        if (str_len + 1 > str_len) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(list[i * 2 + 1], str, str_len + 1);
        } else {
            free(list[i * 2 + 1]);
            for (int j = 0; j <= i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
    }

    return list;
}

void free_list(char** list, int size) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < size * 2; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char* tuple[] = {"apple", "banana", "cherry"};
    int size = sizeof(tuple) / sizeof(tuple[0]);
    const char* str = "-";

    char** list = tuple_to_list((char**)tuple, size, str);
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < size * 2; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list, size);
    return 0;
}