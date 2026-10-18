#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_STRING_MAX 4096

static size_t safe_strlen(const char *s) {
    if (s == NULL) return 0;
    return strnlen(s, SAFE_STRING_MAX);
}

char** tuple_to_list(const char** tuple, int size, const char* str) {
    if (tuple == NULL || str == NULL || size <= 0) {
        return NULL;
    }

    char** list = malloc((size * 2) * sizeof(char*));
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

        size_t len1 = safe_strlen(tuple[i]);
        list[i * 2] = malloc(len1 + 1);
        if (list[i * 2] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        snprintf(list[i * 2], len1 + 1, "%s", tuple[i]);

        size_t len2 = safe_strlen(str);
        list[i * 2 + 1] = malloc(len2 + 1);
        if (list[i * 2 + 1] == NULL) {
            for (int j = 0; j <= i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        snprintf(list[i * 2 + 1], len2 + 1, "%s", str);
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

    char** list = tuple_to_list(tuple, size, str);
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