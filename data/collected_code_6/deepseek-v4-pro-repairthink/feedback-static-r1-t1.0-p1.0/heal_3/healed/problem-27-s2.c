#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_digits(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    char *result = strdup(str);
    if (result == NULL) {
        return NULL;
    }
    size_t j = 0;
    for (size_t i = 0; result[i] != '\0'; i++) {
        if (!isdigit((unsigned char)result[i])) {
            result[j++] = result[i];
        }
    }
    result[j] = '\0';
    char *shrunk = realloc(result, j + 1);
    return shrunk != NULL ? shrunk : result;
}

void remove_digits_from_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            char *cleaned = remove_digits(list[i]);
            if (cleaned != NULL) {
                free(list[i]);
                list[i] = cleaned;
            }
        }
    }
}

int main(void) {
    const char *src[] = {"abc123def", "456", "no digits here", "a1b2c3d4e5"};
    size_t count = sizeof(src) / sizeof(src[0]);

    char **strings = malloc(count * sizeof(char *));
    if (strings == NULL) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < count; i++) {
        strings[i] = NULL;
    }

    for (size_t i = 0; i < count; i++) {
        strings[i] = strdup(src[i]);
        if (strings[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            return EXIT_FAILURE;
        }
    }

    remove_digits_from_list(strings, count);

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
        free(strings[i]);
    }
    free(strings);

    return EXIT_SUCCESS;
}