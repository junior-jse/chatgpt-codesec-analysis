#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str, size_t bufsize) {
    if (str == NULL || bufsize == 0) {
        return;
    }
    size_t len = strnlen(str, bufsize);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void reverse_strings(char **strings, const size_t *sizes, size_t count) {
    if (strings == NULL || sizes == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        reverse_string(strings[i], sizes[i]);
    }
}

int main(void) {
    const size_t count = 4;
    /* Possible weaknesses found:
     *  Variable 'sizes' can be declared as const array [constVariable]
     */
    size_t sizes[] = {6, 8, 6, 7};

    char **list = malloc(count * sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    list[0] = malloc(sizes[0]);
    list[1] = malloc(sizes[1]);
    list[2] = malloc(sizes[2]);
    list[3] = malloc(sizes[3]);

    if (list[0] == NULL || list[1] == NULL || list[2] == NULL || list[3] == NULL) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }

    snprintf(list[0], sizes[0], "%s", "hello");
    snprintf(list[1], sizes[1], "%s", "reverse");
    snprintf(list[2], sizes[2], "%s", "world");
    snprintf(list[3], sizes[3], "%s", "string");

    reverse_strings(list, sizes, count);

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    for (size_t i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);

    return EXIT_SUCCESS;
}