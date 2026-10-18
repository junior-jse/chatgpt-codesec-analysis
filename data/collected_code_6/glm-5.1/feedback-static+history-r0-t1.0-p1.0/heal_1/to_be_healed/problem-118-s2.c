#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_to_list(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len == 0) {
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
    return list;
}

void free_list(char **list) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *str = "hello";
    char **list = string_to_list(str);

    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; list[i] != NULL; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list);

    return EXIT_SUCCESS;
}