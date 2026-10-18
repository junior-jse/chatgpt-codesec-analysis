#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len1 = strlen(tuple[i]);
        list[i * 2] = malloc(len1 + 1);
        if (list[i * 2] == NULL) {
            for (int j = 0; j < i * 2; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        snprintf(list[i * 2], len1 + 1, "%s", tuple[i]);

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len2 = strlen(str);
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